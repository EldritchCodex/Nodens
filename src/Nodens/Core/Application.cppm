/// @file Application.cppm
/// @brief Core application class and startup configuration for the Nodens framework.
/// @ingroup Core

export module Nodens.Application;

import Nodens.Events;
import Nodens.JobSystem;
import Nodens.Layer;
import Nodens.LayerStack;
import Nodens.ImGuiLayer;
import Nodens.Window;
import Nodens.DefaultTheme;
import std;

export namespace Nodens
{

/// @brief Configuration data used to initialize an Application instance.
/// @details Provides sensible defaults for a windowed GUI application.
///          Pass a customized instance to the Application constructor to override.
/// @ingroup Core
struct ApplicationSpecification
{
    std::string Name{"Nodens Application"};          ///< Window title and application identifier.
    std::uint32_t WindowWidth{1280};                 ///< Initial window width in pixels.
    std::uint32_t WindowHeight{720};                 ///< Initial window height in pixels.
    bool EnableGUI{true};                            ///< Whether to create the ImGui overlay layer.
    bool IsHeadless{false};                          ///< If true, no window or graphics context is created.
    EDefaultTheme DefaultTheme{EDefaultTheme::Dark}; ///< The visual theme applied to ImGui on startup.
};

/// @brief The central singleton that owns the window, layer stack, job system, and main loop.
/// @details A client application subclasses Application and pushes its own Layer instances
///          in the constructor. Exactly one Application instance may exist at any time;
///          creating a second one terminates the process.
///
/// **Typical usage:**
/// @code
/// class MyApp : public Nodens::Application {
/// public:
///     MyApp() : Application({.Name = "Demo"}) {
///         PushLayer(new MyLayer());
///     }
/// };
/// int main() {
///     Nodens::InitializeLoggers();
///     MyApp app;
///     app.Run();
/// }
/// @endcode
///
/// @see Layer, LayerStack, ApplicationSpecification
/// @ingroup Core
class Application
{
public:
    /// @brief Constructs the application, creating the window, job system, and optional ImGui layer.
    /// @param specification The configuration to use for initialization.
    explicit Application(const ApplicationSpecification& specification);

    virtual ~Application();

    /// @brief Enters the main loop and runs until the window is closed.
    /// @details Each frame updates every layer, renders ImGui, and swaps buffers.
    ///          The loop is driven by a monotonic clock for frame-time calculation.
    void Run();

    /// @brief Dispatches an event through the layer stack (back-to-front).
    /// @param e The event to dispatch. May be marked as handled by a layer.
    void OnEvent(Event& e);

    /// @brief Pushes a regular layer onto the layer stack and calls its OnAttach().
    /// @param layer Raw pointer to the layer. Ownership is transferred to the LayerStack.
    void PushLayer(Layer* layer);

    /// @brief Pushes an overlay layer (rendered last) and calls its OnAttach().
    /// @param overlay Raw pointer to the overlay. Ownership is transferred to the LayerStack.
    void PushOverlay(Layer* overlay);

    /// @brief Returns a reference to the application window.
    /// @warning Calling this on a headless application triggers a fatal error.
    /// @return Reference to the Window instance.
    Window& GetWindow();

    /// @brief Returns a reference to the application's job system.
    /// @return Reference to the JobSystem instance.
    JobSystem& GetJobSystem();

    /// @brief Returns the specification used to initialize this application.
    /// @return Const reference to the ApplicationSpecification.
    const ApplicationSpecification& GetSpecification() const;

    /// @brief Retrieves the global singleton Application instance.
    /// @return Reference to the running Application.
    static Application& Get();

private:
    /// @brief Handles the WindowCloseEvent by setting the running flag to false.
    /// @param e The window close event.
    /// @return Always returns true (event is consumed).
    bool OnWindowClose(WindowCloseEvent& e);

    ApplicationSpecification m_Specification; ///< Stored copy of the startup configuration.
    bool m_Running = true;                    ///< Main loop sentinel; false triggers shutdown.

    std::unique_ptr<Window> m_Window;         ///< The platform window (null in headless mode).
    std::unique_ptr<ImGuiLayer> m_ImGuiLayer; ///< The ImGui overlay (null when GUI is disabled).
    std::unique_ptr<LayerStack> m_LayerStack; ///< Ordered collection of active layers.

    std::unique_ptr<JobSystem> m_JobSystem; ///< The multithreaded job system.

    float m_LastFrameTime{0.0f}; ///< Timestamp of the previous frame (seconds since start).

private:
    static Application* s_Instance; ///< Pointer to the sole Application instance.
};

} // namespace Nodens
