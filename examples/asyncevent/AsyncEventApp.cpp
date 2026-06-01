import Nodens.Application;
import Nodens.Log;
import Example.AsyncEventLayer;

class AsyncEventApp : public Nodens::Application
{
public:
    AsyncEventApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new AsyncEventLayer());
    }

    ~AsyncEventApp() {}
};

namespace Nodens
{
Application* CreateApplication()
{
    return new AsyncEventApp({
        .Name         = "[NodensApp Example] AsyncEvents",
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