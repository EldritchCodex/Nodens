#include "Nodens/Application.h"
#include "ndpch.h"

#include <chrono>
#include <ranges>

#include "Nodens/Events/ApplicationEvent.h"
#include "Nodens/Input.h"
#include "Nodens/LayerStack.h"
#include "Nodens/Log.h"
#include "Nodens/imgui/ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLImGuiRenderer.h"

namespace Nodens
{

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
{
    ZoneScoped;

    // Ensure strictly one Application instance exists
    ND_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_JobSystem  = std::make_unique<JobSystem>();
    m_LayerStack = std::make_unique<LayerStack>();

    if (!m_Specification.IsHeadless)
    {
        WindowProps props(m_Specification.Name, m_Specification.WindowWidth, m_Specification.WindowHeight);
        m_Window = std::unique_ptr<Window>(Window::Create(props));
        m_Window->SetEventCallback(ND_BIND_EVENT_FN(Application::OnEvent));
    }

    if (m_Specification.EnableGUI && !m_Specification.IsHeadless)
    {
        std::shared_ptr<ImGuiRenderer> imguiRenderer = std::make_shared<OpenGLImGuiRenderer>();
        m_ImGuiLayer                                 = std::make_unique<ImGuiLayer>(imguiRenderer);
        PushOverlay(m_ImGuiLayer.get());
    }
}

Application::~Application()
{
    ZoneScoped;
    // Cleanup logic if necessary
}

void Application::PushLayer(Layer* layer)
{
    ZoneScoped;
    m_LayerStack->PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
    ZoneScoped;
    m_LayerStack->PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::Run()
{
    const auto startTime = std::chrono::steady_clock::now();

    while (m_Running)
    {
        ZoneScoped;

        const auto currentTime = std::chrono::steady_clock::now();
        float      time        = std::chrono::duration<float>(currentTime - startTime).count();
        TimeStep   timestep    = time - m_LastFrameTime;
        m_LastFrameTime        = time;

        // Update each layer
        for (Layer* layer : *m_LayerStack)
            layer->OnUpdate(timestep);

        if (m_ImGuiLayer)
        {
            m_ImGuiLayer->Begin();
            for (Layer* layer : *m_LayerStack)
                layer->OnImGuiRender(timestep);
            m_ImGuiLayer->End();
        }

        if (m_Window)
            m_Window->OnUpdate();

        FrameMark;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

void Application::OnEvent(Event& e)
{
    ZoneScoped;

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(ND_BIND_EVENT_FN(Application::OnWindowClose));

    // Run through LayerStack from last to first
    for (auto layer : *m_LayerStack | std::views::reverse)
    {
        layer->OnEvent(e);
        if (e.Handled)
            break;
    }
}

} // namespace Nodens