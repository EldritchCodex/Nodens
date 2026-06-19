/// @file Window.cppm
/// @brief Abstract window interface and creation properties.
/// @ingroup Rendering

export module Nodens.Window;

import Nodens.Events;
import std;

export namespace Nodens
{

/// @brief Configuration properties for creating a Window.
/// @details Passed to the static Window::Create() factory. Provides sensible defaults
///          for title, dimensions, and VSync.
/// @ingroup Rendering
struct WindowProps
{
    std::string Title;   ///< The window title displayed in the title bar.
    unsigned int Width;  ///< Initial window width in pixels.
    unsigned int Height; ///< Initial window height in pixels.
    bool VSync;          ///< Whether vertical synchronization is enabled.

    /// @brief Constructs window properties with optional overrides.
    /// @param title  Window title.
    /// @param width  Window width in pixels.
    /// @param height Window height in pixels.
    /// @param vsync  Enable VSync.
    WindowProps(
        const std::string& title = "[Nodens]", unsigned int width = 1280, unsigned int height = 720, bool vsync = true)
        : Title(title), Width(width), Height(height), VSync(vsync)
    {
    }
};

/// @brief Abstract base class for a platform window.
/// @details Provides a platform-agnostic interface for window management, including
///          update polling, dimension queries, VSync control, event callbacks, and
///          access to the underlying native window handle.
///
///          The concrete implementation (e.g., GlfwWindow) is created via the static
///          factory method Create().
/// @see WindowProps, GlfwWindow
/// @ingroup Rendering
class Window
{
public:
    /// @brief Type alias for the event callback function.
    /// @details The Application sets this callback; the window implementation invokes it
    ///          whenever a platform event (resize, close, key, mouse) occurs.
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window()
    {
    }

    /// @brief Polls platform events and swaps buffers. Called once per frame.
    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    /// @brief Sets the event callback invoked by the window on platform events.
    /// @param callback The function to call with each Event.
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

    /// @brief Enables or disables vertical synchronization.
    /// @param enabled True to enable VSync, false to disable.
    virtual void SetVSync(bool enabled) = 0;

    /// @brief Queries whether VSync is currently enabled.
    /// @return True if VSync is on.
    virtual bool IsVSyncOn() const = 0;

    /// @brief Returns a raw pointer to the underlying native window handle.
    /// @details For GLFW this returns a `GLFWwindow*`. Cast the result to the
    ///          appropriate type.
    /// @return Opaque pointer to the native window.
    virtual void* GetNativeWindow() const = 0;

    /// @brief Static factory method that creates a platform-specific Window.
    /// @param props The configuration properties for the new window.
    /// @return A raw pointer to the newly created Window. Caller takes ownership.
    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace Nodens
