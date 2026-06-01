import Nodens.Application;
import Nodens.Log;
import Example.JobSystemLayer;

class JobSystemApp : public Nodens::Application
{
public:
    JobSystemApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new JobSystemLayer());
    }

    ~JobSystemApp() {}
};

namespace Nodens
{
Application* CreateApplication()
{
    return new JobSystemApp({
        .Name         = "[NodensApp Example] JobSystem",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    });
}
} // namespace Nodens

#ifndef ND_EXCLUDE_APP_MAIN
int main()
{
    Nodens::InitializeLogging();

    auto app = Nodens::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
#endif