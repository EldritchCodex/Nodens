/// @file VulkanContext.cppm
/// @brief Nodens-owned Vulkan instance, device, surface, and swapchain context.
/// @details Nodens owns loader, instance, surface, physical-device selection,
///          logical-device creation, and swapchain resources. Frame scheduling
///          follows later.
/// @ingroup Platform

module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cassert>

export module Nodens.VulkanContext;

import Nodens.GraphicsContext;
import Nodens.Log;
import std;
export import vulkan;

export namespace Nodens
{
/// @brief Vulkan context shared by Nodens and an attached renderer.
/// @details Nodens owns Vulkan platform and swapchain resources. Consumers borrow
///          these handles and must not destroy them. The context requires a GLFW window
///          created with `GLFW_NO_API`.
/// @ingroup Platform
class VulkanContext : public IGraphicsContext
{
public:
    static constexpr uint32_t FramesInFlight{2};
    /// @brief Constructs a Vulkan context for an existing GLFW window.
    /// @param windowHandle A valid GLFW window created without a client API.
    explicit VulkanContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        if (!m_WindowHandle)
            throw std::invalid_argument{"VulkanContext requires a valid GLFW window"};
    }

    ~VulkanContext() override = default;

    /// @brief Creates the Vulkan instance, surface, physical device, and logical device.
    void Init() override
    {
        if (static_cast<VkInstance>(*m_Instance) != VK_NULL_HANDLE)
            return;

        uint32_t extensionCount{0};
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        if (!requiredExtensions)
            throw std::runtime_error{"GLFW Vulkan instance extensions are unavailable"};

        std::vector<const char*> extensions(requiredExtensions,
                                            requiredExtensions + extensionCount);
        constexpr vk::ApplicationInfo applicationInfo{
            .pApplicationName = "Nodens",
            .applicationVersion = vk::makeApiVersion(0, 1, 0, 0),
            .pEngineName = "Nodens",
            .engineVersion = vk::makeApiVersion(0, 1, 0, 0),
            .apiVersion = vk::ApiVersion13,
        };
        const vk::InstanceCreateInfo instanceCreateInfo{
            .pApplicationInfo = &applicationInfo,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        m_Instance = vk::raii::Instance{m_Context, instanceCreateInfo};

        VkSurfaceKHR rawSurface{VK_NULL_HANDLE};
        if (glfwCreateWindowSurface(*m_Instance, m_WindowHandle, nullptr, &rawSurface) !=
            VK_SUCCESS)
            throw std::runtime_error{"Failed to create Vulkan window surface"};

        m_Surface = vk::raii::SurfaceKHR{m_Instance, rawSurface};
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapchain();
        CreateCommandResources();
        CreateFrameSynchronization();
    }

    /// @brief Completes, submits, and presents the active frame.
    void Present() override
    {
        if (!m_FrameStarted)
            return;

        TransitionActiveImageToPresent();
        auto& commandBuffer = m_CommandBuffers[m_CurrentFrameIndex];
        commandBuffer.end();
        const vk::CommandBuffer rawCommandBuffer = *commandBuffer;

        const vk::PipelineStageFlags waitStage{vk::PipelineStageFlagBits::eColorAttachmentOutput};
        const vk::SubmitInfo submitInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*m_PresentCompleteSemaphores[m_CurrentFrameIndex],
            .pWaitDstStageMask = &waitStage,
            .commandBufferCount = 1,
            .pCommandBuffers = &rawCommandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &*m_RenderFinishedSemaphores[m_CurrentImageIndex],
        };
        m_GraphicsQueue.submit(submitInfo, *m_InFlightFences[m_CurrentFrameIndex]);

        const vk::PresentInfoKHR presentInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*m_RenderFinishedSemaphores[m_CurrentImageIndex],
            .swapchainCount = 1,
            .pSwapchains = &*m_Swapchain,
            .pImageIndices = &m_CurrentImageIndex,
        };
        const auto result = m_GraphicsQueue.presentKHR(presentInfo);
        m_FrameStarted = false;

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
            RecreateSwapchain();
        else if (result != vk::Result::eSuccess)
            throw std::runtime_error{"Nodens failed to present swapchain image"};
        else
            m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % FramesInFlight;
    }

    /// @brief Returns the Vulkan instance owned by Nodens.
    /// @return Borrowed Vulkan instance handle.
    vk::Instance GetInstance() const
    {
        return *m_Instance;
    }

    /// @brief Returns the Vulkan surface owned by Nodens.
    /// @return Borrowed Vulkan surface handle.
    vk::SurfaceKHR GetSurface() const
    {
        return *m_Surface;
    }

    /// @brief Returns the Vulkan-Hpp RAII instance owned by Nodens.
    /// @return Borrowed reference to the RAII instance.
    const vk::raii::Instance& GetInstanceRAII() const
    {
        return m_Instance;
    }

    /// @brief Returns the Vulkan-Hpp RAII surface owned by Nodens.
    /// @return Borrowed reference to the RAII surface.
    const vk::raii::SurfaceKHR& GetSurfaceRAII() const
    {
        return m_Surface;
    }

    /// @brief Returns the physical device selected by Nodens.
    /// @return Borrowed reference to the selected physical device.
    const vk::raii::PhysicalDevice& GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }

    /// @brief Returns the logical device created by Nodens.
    /// @return Borrowed reference to the logical device.
    const vk::raii::Device& GetDeviceRAII() const
    {
        return m_Device;
    }

    /// @brief Returns the graphics and presentation queue created by Nodens.
    /// @return Borrowed reference to the queue.
    const vk::raii::Queue& GetGraphicsQueueRAII() const
    {
        return m_GraphicsQueue;
    }

    /// @brief Returns the graphics and presentation queue family index.
    uint32_t GetGraphicsQueueFamilyIndex() const
    {
        return m_GraphicsQueueFamilyIndex;
    }

    /// @brief Returns the Nodens-owned swapchain.
    /// @return Borrowed reference to the swapchain.
    const vk::raii::SwapchainKHR& GetSwapchainRAII() const
    {
        return m_Swapchain;
    }

    /// @brief Returns swapchain image handles owned by Nodens.
    /// @return Borrowed reference to swapchain images.
    const std::vector<vk::Image>& GetSwapchainImages() const
    {
        return m_SwapchainImages;
    }

    /// @brief Returns swapchain image views owned by Nodens.
    /// @return Borrowed reference to swapchain image views.
    const std::vector<vk::raii::ImageView>& GetSwapchainImageViews() const
    {
        return m_SwapchainImageViews;
    }

    /// @brief Returns current swapchain extent.
    vk::Extent2D GetSwapchainExtent() const
    {
        return m_SwapchainExtent;
    }

    /// @brief Returns current swapchain surface format.
    vk::SurfaceFormatKHR GetSwapchainSurfaceFormat() const
    {
        return m_SwapchainSurfaceFormat;
    }

    /// @brief Recreates swapchain resources after a surface change.
    void RecreateSwapchain()
    {
        m_Device.waitIdle();
        m_SwapchainImageViews.clear();
        m_RenderFinishedSemaphores.clear();
        m_Swapchain = nullptr;
        CreateSwapchain();
        CreateRenderFinishedSemaphores();
    }

    /// @brief Returns number of frames Nodens schedules concurrently.
    uint32_t GetFramesInFlight() const
    {
        return FramesInFlight;
    }

    /// @brief Returns current frame slot used by Nodens.
    uint32_t GetCurrentFrameIndex() const
    {
        return m_CurrentFrameIndex;
    }

    /// @brief Returns the swapchain image acquired for the active frame.
    uint32_t GetCurrentImageIndex() const
    {
        return m_CurrentImageIndex;
    }

    /// @brief Reports whether BeginFrame() started a frame for recording.
    bool IsFrameActive() const
    {
        return m_FrameStarted;
    }

    /// @brief Returns command buffer receiving commands for the active frame.
    /// @return Borrowed reference to the active RAII command buffer.
    const vk::raii::CommandBuffer& GetActiveCommandBuffer() const
    {
        return m_CommandBuffers[m_CurrentFrameIndex];
    }

    /// @brief Transitions the active swapchain image to the presentation layout.
    /// @details Called by Present() after all renderers finish recording. Renderers can
    ///          therefore append work after Nyar's scene pass without ending the frame.
    void TransitionActiveImageToPresent()
    {
        const vk::ImageMemoryBarrier2 barrier{
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
            .dstAccessMask = {},
            .oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout = vk::ImageLayout::ePresentSrcKHR,
            .srcQueueFamilyIndex = vk::QueueFamilyIgnored,
            .dstQueueFamilyIndex = vk::QueueFamilyIgnored,
            .image = m_SwapchainImages[m_CurrentImageIndex],
            .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                                 .baseMipLevel = 0,
                                 .levelCount = 1,
                                 .baseArrayLayer = 0,
                                 .layerCount = 1},
        };
        const vk::DependencyInfo dependencyInfo{.imageMemoryBarrierCount = 1,
                                                .pImageMemoryBarriers = &barrier};
        m_CommandBuffers[m_CurrentFrameIndex].pipelineBarrier2(dependencyInfo);
        m_SwapchainImageLayouts[m_CurrentImageIndex] = vk::ImageLayout::ePresentSrcKHR;
    }

    /// @brief Waits until all Nodens-owned Vulkan work completes.
    void WaitIdle() const
    {
        m_Device.waitIdle();
    }

    /// @brief Acquires a swapchain image and starts one frame.
    /// @return Image index, or no value when swapchain recreation is required.
    std::optional<uint32_t> BeginFrame()
    {
        int framebufferWidth{0};
        int framebufferHeight{0};
        glfwGetFramebufferSize(m_WindowHandle, &framebufferWidth, &framebufferHeight);
        if (framebufferWidth == 0 || framebufferHeight == 0)
            return std::nullopt;

        // Recreate before recording when a resize leaves the swapchain extent stale.
        const auto surfaceCapabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface);
        const auto expectedExtent = ChooseSwapchainExtent(surfaceCapabilities);
        if (expectedExtent.width != m_SwapchainExtent.width ||
            expectedExtent.height != m_SwapchainExtent.height)
        {
            RecreateSwapchain();
            return std::nullopt;
        }

        const auto fenceResult = m_Device.waitForFences(
            *m_InFlightFences[m_CurrentFrameIndex], vk::True, std::numeric_limits<uint64_t>::max());
        if (fenceResult != vk::Result::eSuccess)
            throw std::runtime_error{"Nodens failed to wait for frame fence"};

        auto [result, imageIndex] =
            m_Swapchain.acquireNextImage(std::numeric_limits<uint64_t>::max(),
                                         *m_PresentCompleteSemaphores[m_CurrentFrameIndex],
                                         nullptr);
        if (result == vk::Result::eErrorOutOfDateKHR)
        {
            RecreateSwapchain();
            return std::nullopt;
        }
        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
            throw std::runtime_error{"Nodens failed to acquire swapchain image"};

        m_Device.resetFences(*m_InFlightFences[m_CurrentFrameIndex]);
        m_CurrentImageIndex = imageIndex;
        m_CommandBuffers[m_CurrentFrameIndex].reset();
        m_CommandBuffers[m_CurrentFrameIndex].begin({});
        TransitionActiveImageToColorAttachment();
        m_FrameStarted = true;
        return imageIndex;
    }

private:
    /// @brief Transitions the acquired swapchain image for color rendering.
    /// @details The acquired image can come from any swapchain slot, so its tracked previous
    ///          layout is used instead of assuming every acquisition starts undefined.
    void TransitionActiveImageToColorAttachment()
    {
        const vk::ImageMemoryBarrier2 barrier{
            .srcStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
            .srcAccessMask = {},
            .dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstAccessMask = vk::AccessFlagBits2::eColorAttachmentRead |
                             vk::AccessFlagBits2::eColorAttachmentWrite,
            .oldLayout = m_SwapchainImageLayouts[m_CurrentImageIndex],
            .newLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .srcQueueFamilyIndex = vk::QueueFamilyIgnored,
            .dstQueueFamilyIndex = vk::QueueFamilyIgnored,
            .image = m_SwapchainImages[m_CurrentImageIndex],
            .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                                 .baseMipLevel = 0,
                                 .levelCount = 1,
                                 .baseArrayLayer = 0,
                                 .layerCount = 1},
        };
        const vk::DependencyInfo dependencyInfo{.imageMemoryBarrierCount = 1,
                                                .pImageMemoryBarriers = &barrier};
        m_CommandBuffers[m_CurrentFrameIndex].pipelineBarrier2(dependencyInfo);
        m_SwapchainImageLayouts[m_CurrentImageIndex] = vk::ImageLayout::eColorAttachmentOptimal;
    }

    /// @brief Checks whether a physical device meets current Nyar requirements.
    bool IsDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice) const
    {
        if (physicalDevice.getProperties().apiVersion < vk::ApiVersion13)
            return false;

        const auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        const bool supportsGraphicsAndPresentation =
            std::ranges::any_of(std::views::iota(size_t{0}, queueFamilies.size()),
                                [&](size_t index)
                                {
                                    return static_cast<bool>(queueFamilies[index].queueFlags &
                                                             vk::QueueFlagBits::eGraphics) &&
                                           physicalDevice.getSurfaceSupportKHR(
                                               static_cast<uint32_t>(index), *m_Surface);
                                });
        if (!supportsGraphicsAndPresentation)
            return false;

        const std::vector<const char*> requiredExtensions{vk::KHRSwapchainExtensionName};
        const auto availableExtensions =
            physicalDevice.enumerateDeviceExtensionProperties() |
            std::views::transform([](const auto& property)
                                  { return std::string_view{property.extensionName}; });
        if (!std::ranges::all_of(requiredExtensions,
                                 [&](std::string_view required)
                                 { return std::ranges::contains(availableExtensions, required); }))
            return false;

        const auto features =
            physicalDevice
                .template getFeatures2<vk::PhysicalDeviceFeatures2,
                                       vk::PhysicalDeviceVulkan11Features,
                                       vk::PhysicalDeviceVulkan13Features,
                                       vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        return features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
               features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
               features.template get<vk::PhysicalDeviceVulkan13Features>().synchronization2 &&
               features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                   .extendedDynamicState;
    }

    /// @brief Selects the first physical device meeting current requirements.
    void PickPhysicalDevice()
    {
        const auto availablePhysicalDevices = m_Instance.enumeratePhysicalDevices();
        const auto deviceIterator = std::ranges::find_if(
            availablePhysicalDevices, [&](const auto& device) { return IsDeviceSuitable(device); });
        if (deviceIterator == availablePhysicalDevices.end())
            throw std::runtime_error{"Nodens found no suitable Vulkan physical device"};

        m_PhysicalDevice = *deviceIterator;

        const vk::PhysicalDeviceProperties properties = m_PhysicalDevice.getProperties();
        CoreLogger().info("Vulkan Info:");
        CoreLogger().info("  GPU Used: {}", std::string_view{properties.deviceName});
        CoreLogger().info("  Vendor ID: 0x{:04x}", properties.vendorID);
        CoreLogger().info("  Device Type: {}", vk::to_string(properties.deviceType));
        CoreLogger().info("  API Version: {}.{}.{}",
                          vk::apiVersionMajor(properties.apiVersion),
                          vk::apiVersionMinor(properties.apiVersion),
                          vk::apiVersionPatch(properties.apiVersion));
        CoreLogger().info("  Driver Version: {}", properties.driverVersion);
    }

    /// @brief Prefers an UNORM color format for Dear ImGui's linear shader output.
    vk::SurfaceFormatKHR
    ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
    {
        assert(!availableFormats.empty());
        const auto formatIterator =
            std::ranges::find_if(availableFormats,
                                 [](const vk::SurfaceFormatKHR& format)
                                 {
                                     return format.format == vk::Format::eB8G8R8A8Unorm &&
                                            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
                                 });
        return formatIterator != availableFormats.end() ? *formatIterator
                                                        : availableFormats.front();
    }

    /// @brief Prefers mailbox presentation and falls back to FIFO.
    vk::PresentModeKHR
    ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const
    {
        assert(std::ranges::any_of(availablePresentModes,
                                   [](vk::PresentModeKHR mode)
                                   { return mode == vk::PresentModeKHR::eFifo; }));
        return std::ranges::any_of(availablePresentModes,
                                   [](vk::PresentModeKHR mode)
                                   { return mode == vk::PresentModeKHR::eMailbox; })
                   ? vk::PresentModeKHR::eMailbox
                   : vk::PresentModeKHR::eFifo;
    }

    /// @brief Clamps framebuffer size to surface limits when needed.
    vk::Extent2D ChooseSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        int width{0};
        int height{0};
        glfwGetFramebufferSize(m_WindowHandle, &width, &height);
        return {.width = std::clamp<uint32_t>(
                    width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                .height = std::clamp<uint32_t>(height,
                                               capabilities.minImageExtent.height,
                                               capabilities.maxImageExtent.height)};
    }

    /// @brief Chooses at least three swapchain images within surface limits.
    uint32_t ChooseSwapchainImageCount(const vk::SurfaceCapabilitiesKHR& capabilities) const
    {
        auto imageCount = std::max(3u, capabilities.minImageCount);
        if (capabilities.maxImageCount != 0 && capabilities.maxImageCount < imageCount)
            imageCount = capabilities.maxImageCount;
        return imageCount;
    }

    /// @brief Creates command pool and one command buffer per frame in flight.
    void CreateCommandResources()
    {
        const vk::CommandPoolCreateInfo poolCreateInfo{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = m_GraphicsQueueFamilyIndex,
        };
        m_CommandPool = vk::raii::CommandPool{m_Device, poolCreateInfo};

        const vk::CommandBufferAllocateInfo allocateInfo{
            .commandPool = *m_CommandPool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = FramesInFlight,
        };
        m_CommandBuffers = vk::raii::CommandBuffers{m_Device, allocateInfo};
    }

    /// @brief Creates synchronization objects for each frame in flight.
    void CreateFrameSynchronization()
    {
        CreateRenderFinishedSemaphores();
        for (uint32_t index = 0; index < FramesInFlight; ++index)
        {
            m_PresentCompleteSemaphores.emplace_back(m_Device, vk::SemaphoreCreateInfo{});
            m_InFlightFences.emplace_back(
                m_Device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled});
        }
    }

    /// @brief Creates one render-finished semaphore per swapchain image.
    void CreateRenderFinishedSemaphores()
    {
        m_RenderFinishedSemaphores.clear();
        for (size_t index = 0; index < m_SwapchainImages.size(); ++index)
            m_RenderFinishedSemaphores.emplace_back(m_Device, vk::SemaphoreCreateInfo{});
    }

    /// @brief Creates swapchain and image views for the current surface.
    void CreateSwapchain()
    {
        const auto capabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface);
        m_SwapchainExtent = ChooseSwapchainExtent(capabilities);
        const auto formats = m_PhysicalDevice.getSurfaceFormatsKHR(*m_Surface);
        m_SwapchainSurfaceFormat = ChooseSwapchainSurfaceFormat(formats);
        const auto presentModes = m_PhysicalDevice.getSurfacePresentModesKHR(*m_Surface);

        const vk::SwapchainCreateInfoKHR createInfo{
            .surface = *m_Surface,
            .minImageCount = ChooseSwapchainImageCount(capabilities),
            .imageFormat = m_SwapchainSurfaceFormat.format,
            .imageColorSpace = m_SwapchainSurfaceFormat.colorSpace,
            .imageExtent = m_SwapchainExtent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform = capabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = ChooseSwapchainPresentMode(presentModes),
            .clipped = true,
        };

        m_Swapchain = vk::raii::SwapchainKHR{m_Device, createInfo};
        m_SwapchainImages = m_Swapchain.getImages();
        // A recreated swapchain has new images, so reset layout tracking with each image set.
        m_SwapchainImageLayouts.assign(m_SwapchainImages.size(), vk::ImageLayout::eUndefined);

        const vk::ImageViewCreateInfo imageViewInfo{
            .viewType = vk::ImageViewType::e2D,
            .format = m_SwapchainSurfaceFormat.format,
            .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                                 .baseMipLevel = 0,
                                 .levelCount = 1,
                                 .baseArrayLayer = 0,
                                 .layerCount = 1},
        };
        for (const auto image : m_SwapchainImages)
        {
            auto viewInfo = imageViewInfo;
            viewInfo.image = image;
            m_SwapchainImageViews.emplace_back(m_Device, viewInfo);
        }
    }

    /// @brief Creates logical device with one graphics-and-present queue.
    void CreateLogicalDevice()
    {
        const auto queueFamilies = m_PhysicalDevice.getQueueFamilyProperties();
        const auto queueIterator =
            std::ranges::find_if(std::views::iota(size_t{0}, queueFamilies.size()),
                                 [&](size_t index)
                                 {
                                     return static_cast<bool>(queueFamilies[index].queueFlags &
                                                              vk::QueueFlagBits::eGraphics) &&
                                            m_PhysicalDevice.getSurfaceSupportKHR(
                                                static_cast<uint32_t>(index), *m_Surface);
                                 });
        if (queueIterator == std::views::iota(size_t{0}, queueFamilies.size()).end())
            throw std::runtime_error{"Nodens found no graphics and presentation queue"};

        m_GraphicsQueueFamilyIndex = static_cast<uint32_t>(*queueIterator);
        constexpr float queuePriority{1.0f};
        const vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = m_GraphicsQueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };

        vk::StructureChain<vk::PhysicalDeviceFeatures2,
                           vk::PhysicalDeviceVulkan11Features,
                           vk::PhysicalDeviceVulkan13Features,
                           vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
            featureChain = {{},
                            {.shaderDrawParameters = true},
                            {.synchronization2 = true, .dynamicRendering = true},
                            {.extendedDynamicState = true}};

        const std::vector<const char*> requiredExtensions{vk::KHRSwapchainExtensionName};
        const vk::DeviceCreateInfo deviceCreateInfo{
            .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
            .ppEnabledExtensionNames = requiredExtensions.data(),
        };

        m_Device = vk::raii::Device{m_PhysicalDevice, deviceCreateInfo};
        m_GraphicsQueue = vk::raii::Queue{m_Device, m_GraphicsQueueFamilyIndex, 0};
    }

    GLFWwindow* m_WindowHandle{nullptr};                ///< Borrowed GLFW window owned by IWindow.
    vk::raii::Context m_Context{};                      ///< Vulkan loader context.
    vk::raii::Instance m_Instance{nullptr};             ///< Nodens-owned Vulkan instance.
    vk::raii::SurfaceKHR m_Surface{nullptr};            ///< Nodens-owned Vulkan surface.
    vk::raii::PhysicalDevice m_PhysicalDevice{nullptr}; ///< Nodens-selected physical device.
    vk::raii::Device m_Device{nullptr};                 ///< Nodens-owned logical device.
    vk::raii::Queue m_GraphicsQueue{nullptr};           ///< Graphics and presentation queue.
    uint32_t m_GraphicsQueueFamilyIndex{};              ///< Graphics and presentation queue family.
    vk::raii::SwapchainKHR m_Swapchain{nullptr};        ///< Nodens-owned presentation swapchain.
    vk::Extent2D m_SwapchainExtent{};                   ///< Current swapchain dimensions.
    vk::SurfaceFormatKHR m_SwapchainSurfaceFormat{};    ///< Current swapchain format.
    std::vector<vk::Image> m_SwapchainImages{};         ///< Swapchain image handles.
    std::vector<vk::ImageLayout> m_SwapchainImageLayouts{}; ///< Current layout per swapchain image.
    std::vector<vk::raii::ImageView> m_SwapchainImageViews{}; ///< Nodens-owned image views.
    vk::raii::CommandPool m_CommandPool{nullptr};             ///< Nodens-owned command pool.
    std::vector<vk::raii::CommandBuffer>
        m_CommandBuffers{}; ///< Nodens-owned frame command buffers.
    std::vector<vk::raii::Semaphore> m_PresentCompleteSemaphores{}; ///< Image-acquire signals.
    std::vector<vk::raii::Semaphore> m_RenderFinishedSemaphores{};  ///< Render-complete signals.
    std::vector<vk::raii::Fence> m_InFlightFences{};                ///< CPU/GPU frame fences.
    uint32_t m_CurrentFrameIndex{0}; ///< Current frame-in-flight slot.
    uint32_t m_CurrentImageIndex{0}; ///< Current acquired image.
    bool m_FrameStarted{false};      ///< BeginFrame has acquired an image.
};
} // namespace Nodens
