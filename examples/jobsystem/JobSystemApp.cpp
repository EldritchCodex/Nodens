#define MAIN_APPLICATION_FILE

#include "JobSystemLayer.h"
#include "nodens.h"

class JobSystemApp : public Nodens::Application
{
public:
    JobSystemApp() { PushLayer(new JobSystemLayer()); }

    JobSystemApp(const Nodens::WindowProps props) : Application(props) { PushLayer(new JobSystemLayer()); }

    ~JobSystemApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new JobSystemApp(Nodens::WindowProps("[NodensApp Example] JobSystem", 800, 600, false));
}