#define MAIN_APPLICATION_FILE

#include "AsyncEventLayer.h"
#include "nodens.h"

class AsyncEventApp : public Nodens::Application
{
public:
    AsyncEventApp() { PushLayer(new AsyncEventLayer()); }

    AsyncEventApp(const Nodens::WindowProps props) : Application(props) { PushLayer(new AsyncEventLayer()); }

    ~AsyncEventApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new AsyncEventApp(Nodens::WindowProps("[NodensApp Example] AsyncEvents", 800, 600, false));
}