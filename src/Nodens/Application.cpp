#include "Application.h"

#include <glfw/glfw3.h>
#include <ranges>

#include "Events/ApplicationEvent.h"
#include "Input.h"
#include "Log.h"
#include "Platform/OpenGL/OpenGLImGuiRenderer.h"
#include "Profiling.h"
#include "ndpch.h"

namespace Nodens
{

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& props)
{
    ND_PROFILE_ZONE_SCOPED;

    // Ensure strictly one Application instance exists
    ND_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    // Initialize subsystems
    m_JobSystem = std::make_unique<JobSystem>();

    // Create the window using the passed properties (or defaults)
    // Window::Create returns a raw pointer, which we immediately wrap in a unique_ptr for ownership.
    m_Window = std::unique_ptr<Window>(Window::Create(props));

    // Set up the event callback to route window events to Application::OnEvent
    m_Window->SetEventCallback(ND_BIND_EVENT_FN(Application::OnEvent));

    // Create the OpenGL implementation.
    // In the future, this can be switched based on config or compile flags.
    std::shared_ptr<ImGuiRenderer> imguiRenderer = std::make_shared<OpenGLImGuiRenderer>();

    m_ImGuiLayer = new ImGuiLayer(imguiRenderer);
    PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
    ND_PROFILE_ZONE_SCOPED;
    // Cleanup logic if necessary (e.g. if LayerStack doesn't own layers)
}

void Application::PushLayer(Layer* layer)
{
    ND_PROFILE_ZONE_SCOPED;
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
    ND_PROFILE_ZONE_SCOPED;
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::Run()
{
    while (m_Running)
    {
        ND_PROFILE_ZONE_SCOPED;

        float    time     = (float)glfwGetTime();
        TimeStep timestep = time - m_LastFrameTime;
        m_LastFrameTime   = time;

        // Update each layer
        for (Layer* layer : m_LayerStack)
            layer->OnUpdate(timestep);

        // ImGui Rendering
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender(timestep);
        m_ImGuiLayer->End();

        m_Window->OnUpdate();

        ND_PROFILE_FRAME_MARK;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

void Application::OnEvent(Event& e)
{
    ND_PROFILE_ZONE_SCOPED;

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(ND_BIND_EVENT_FN(Application::OnWindowClose));

    // Run through LayerStack from last to first
    for (auto layer : m_LayerStack | std::views::reverse)
    {
        layer->OnEvent(e);
        if (e.Handled)
            break;
    }
}

} // namespace Nodens