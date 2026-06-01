export module Nodens.Application;

import Nodens.Events;
import Nodens.JobSystem;
import Nodens.Layer;
import Nodens.LayerStack;
import Nodens.ImGuiLayer;
import Nodens.Window;
import std;

export namespace Nodens
{

struct ApplicationSpecification
{
    std::string   Name         = "Nodens Application";
    std::uint32_t WindowWidth  = 1280;
    std::uint32_t WindowHeight = 720;
    bool          EnableGUI    = true;
    bool          IsHeadless   = false;
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

    Window&                         GetWindow();
    JobSystem&                      GetJobSystem();
    const ApplicationSpecification& GetSpecification() const;

    static Application& Get();

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

} // namespace Nodens
