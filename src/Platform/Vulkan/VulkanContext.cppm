/// @file VulkanContext.cppm
/// @brief Nodens-owned Vulkan instance, surface, and physical-device context.
/// @details Nodens owns loader, instance, surface, and physical-device selection.
///          Logical device, swapchain, and frame scheduling follow later.
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

    /// @brief Creates the Vulkan instance, window surface, and physical device.
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

    GLFWwindow* m_WindowHandle{nullptr};                ///< Borrowed GLFW window owned by IWindow.
    vk::raii::Context m_Context{};                      ///< Vulkan loader context.
    vk::raii::Instance m_Instance{nullptr};             ///< Nodens-owned Vulkan instance.
    vk::raii::SurfaceKHR m_Surface{nullptr};            ///< Nodens-owned window surface.
    vk::raii::PhysicalDevice m_PhysicalDevice{nullptr}; ///< Nodens-selected physical device.
};
} // namespace Nodens
