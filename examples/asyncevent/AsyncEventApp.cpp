import Nodens;
import Example.AsyncEventLayer;

// -----------------------------------------------------------------------------
// Step 3: Define the Application class, the root object in charge of the
// application, managing the window, layers, and the main event loop.
// -----------------------------------------------------------------------------

class AsyncEventApp : public Nodens::Application
{
public:
    static inline const Nodens::ApplicationSpecification appSpecifications = {
        .Name = "[NodensApp Example] AsyncEvents",
        .WindowWidth = 800,
        .WindowHeight = 600,
        .EnableGUI = true,
        .IsHeadless = false,
    };

    AsyncEventApp() : Application(appSpecifications)
    {
        PushLayer(new AsyncEventLayer());
    }

    ~AsyncEventApp() = default;
};

// -----------------------------------------------------------------------------
// Step 4: Application entry point.
// -----------------------------------------------------------------------------

int main()
{
    Nodens::InitializeLoggers();

    auto app = AsyncEventApp();
    app.Run();

    return 0;
}
