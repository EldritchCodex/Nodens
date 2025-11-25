#include "Application.h"

#include <glfw/glfw3.h>
#include <ranges>

#include "Events/ApplicationEvent.h"
#include "Input.h"
#include "Log.h"
#include "Profiling.h"
#include "ndpch.h"

namespace Nodens
{
Application* Application::s_Instance = nullptr;
// Provides access to Application information
// for the rest of the program.

Application::Application()
{
    ND_PROFILE_ZONE_SCOPED;
    ND_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_JobSystem = std::make_unique<JobSystem>();

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(ND_BIND_EVENT_FN(Application::OnEvent));
    // 1. Events come from the window.
    // 2. We pass it to the application.
    // 3. The application dispatches it to the layers.

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::Application(const WindowProps& props)
{
    ND_CORE_ASSERT(!s_Instance, "Application already exists!");
    ND_PROFILE_ZONE_SCOPED;

    s_Instance  = this;
    m_JobSystem = std::make_unique<JobSystem>();

    m_Window = std::unique_ptr<Window>(Window::Create(props));
    m_Window->SetEventCallback(ND_BIND_EVENT_FN(Application::OnEvent));
    // 1. Events come from the window.
    // 2. We pass it to the application.
    // 3. The application dispatches it to the layers.

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
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

        // ImGUI Rendering
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