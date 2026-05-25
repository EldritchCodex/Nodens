#define MAIN_APPLICATION_FILE

#include "CircularWave3DLayer.h"
#include "nodens.h"

class CircularWave3DApp : public Nodens::Application
{
public:
    CircularWave3DApp(const Nodens::ApplicationSpecification& specification) : Application(specification)
    {
        PushLayer(new CircularWave3DLayer());
    }

    ~CircularWave3DApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new CircularWave3DApp({
        .Name         = "[NodensApp Example] CircularWave3D",
        .WindowWidth  = 800,
        .WindowHeight = 600,
        .EnableGUI    = true,
        .IsHeadless   = false,
    });
}