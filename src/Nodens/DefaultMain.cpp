import Nodens.Log;
import Nodens.Application;
import std;

namespace Nodens
{
Application* CreateApplication();
}

// Optional default entry point for applications that prefer not to write their own main().
// Link an executable against the Nodens::DefaultMain target to use this implementation.
int main()
{
    Nodens::InitializeLogging();

    auto app = Nodens::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
