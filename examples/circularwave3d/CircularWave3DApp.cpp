import Nodens.Application;
import Nodens.Log;
import Example.CircularWave3DLayer;

class CircularWave3DApp : public Nodens::Application
{
public:
    CircularWave3DApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new CircularWave3DLayer());
    }

    ~CircularWave3DApp() {}
};

namespace Nodens
{
Application* CreateApplication()
{
    return new CircularWave3DApp({
        .Name         = "[NodensApp Example] CircularWave3D",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    });
}
} // namespace Nodens

#ifndef ND_EXCLUDE_APP_MAIN
int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Nodens::InitializeLogging();

    auto app = Nodens::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
#endif