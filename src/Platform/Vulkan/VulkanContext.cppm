/// @file VulkanContext.cppm
/// @brief Nodens-owned Vulkan instance, device, surface, and physical-device context.
/// @details Nodens owns loader, instance, surface, physical-device selection, and
///          logical-device creation. Swapchain and frame scheduling follow later.
/// @ingroup Platform

module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module Nodens.VulkanContext;

import Nodens.GraphicsContext;
import Nodens.Log;
import std;
export import vulkan;

export namespace Nodens
{
/// @brief Vulkan context shared by Nodens and an attached renderer.
/// @details Nodens owns the Vulkan instance and surface. Consumers borrow these
///          handles and must not destroy them. The context requires a GLFW window
///          created with `GLFW_NO_API`.
/// @ingroup Platform
class VulkanContext : public IGraphicsContext
{
public:
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
    }

    /// @brief Does nothing until Nodens owns Vulkan frame submission.
    void Present() override
    {
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

private:
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
};
} // namespace Nodens
