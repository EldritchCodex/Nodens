#pragma once

#include "Core.h"
#include "Nodens/Events/ApplicationEvent.h"
#include "Nodens/Events/Event.h"
#include "Nodens/JobSystem.h"
#include "Nodens/Layer.h"
#include "Nodens/TimeStep.h"
#include "Window.h"

#include <cstdint>
#include <memory>
#include <string>

namespace Nodens
{

class LayerStack;
class ImGuiLayer;

struct ApplicationSpecification
{
    std::string Name         = "Nodens Application";
    uint32_t    WindowWidth  = 1280;
    uint32_t    WindowHeight = 720;
    bool        EnableGUI    = true;
    bool        IsHeadless   = false;
};

class Application
{
public:
    explicit Application(const ApplicationSpecification& specification);
    virtual ~Application();

    void Run();
    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    inline Window& GetWindow()
    {
        ND_CORE_ASSERT(m_Window, "Attempted to access Window in a headless application!");
        return *m_Window;
    }

    inline JobSystem&                      GetJobSystem() { return *m_JobSystem; }
    inline const ApplicationSpecification& GetSpecification() const { return m_Specification; }

    static inline Application& Get() { return *s_Instance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

    ApplicationSpecification m_Specification;
    bool                     m_Running = true;

    std::unique_ptr<Window>     m_Window;
    std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
    std::unique_ptr<LayerStack> m_LayerStack;

    std::unique_ptr<JobSystem> m_JobSystem;

    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance;
};

// To be defined in CLIENT
Application* CreateApplication();

} // namespace Nodens