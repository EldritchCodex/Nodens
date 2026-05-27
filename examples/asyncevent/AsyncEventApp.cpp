#define MAIN_APPLICATION_FILE

#include "AsyncEventLayer.h"
#include "nodens.h"

class AsyncEventApp : public Nodens::Application
{
public:
    AsyncEventApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new AsyncEventLayer());
    }

    ~AsyncEventApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new AsyncEventApp({
        .Name         = "[NodensApp Example] AsyncEvents",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    });
}