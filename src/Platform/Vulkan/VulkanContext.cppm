/// @file VulkanContext.cppm
/// @brief Nodens-owned Vulkan instance and window-surface context.
/// @details This first Vulkan integration step owns only loader, instance, and
///          surface state. Device, swapchain, and frame scheduling follow later.
/// @ingroup Platform

module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module Nodens.VulkanContext;

import Nodens.GraphicsContext;
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

    /// @brief Creates the Vulkan instance and window surface.
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

private:
    GLFWwindow* m_WindowHandle{nullptr};     ///< Borrowed GLFW window owned by IWindow.
    vk::raii::Context m_Context{};           ///< Vulkan loader context.
    vk::raii::Instance m_Instance{nullptr};  ///< Nodens-owned Vulkan instance.
    vk::raii::SurfaceKHR m_Surface{nullptr}; ///< Nodens-owned window surface.
};
} // namespace Nodens
