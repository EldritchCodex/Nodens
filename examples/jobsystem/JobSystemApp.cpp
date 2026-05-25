#define MAIN_APPLICATION_FILE

#include "JobSystemLayer.h"
#include "nodens.h"

class JobSystemApp : public Nodens::Application
{
public:
    JobSystemApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new JobSystemLayer());
    }

    ~JobSystemApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new JobSystemApp({
        .Name         = "[NodensApp Example] JobSystem",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    });
}