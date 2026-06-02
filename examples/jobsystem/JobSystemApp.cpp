import Nodens;
import Example.JobSystemLayer;

// -----------------------------------------------------------------------------
// Step 2: Define the Application class, the root object in charge of the
// application, managing the window, layers, and the main event loop.
// -----------------------------------------------------------------------------

class JobSystemApp : public Nodens::Application
{
public:
    static inline const Nodens::ApplicationSpecification appSpecifications = {
        .Name         = "[NodensApp Example] JobSystem",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    };

    JobSystemApp() : Application(appSpecifications) { PushLayer(new JobSystemLayer()); }

    ~JobSystemApp() = default;
};

int main()
{
    Nodens::InitializeLoggers();

    auto app = JobSystemApp();
    app.Run();

    return 0;
}