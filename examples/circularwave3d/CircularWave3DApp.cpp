#define MAIN_APPLICATION_FILE

#include "CircularWave3DLayer.h"
#include "nodens.h"

class CircularWave3DApp : public Nodens::Application
{
public:
    CircularWave3DApp() { PushLayer(new CircularWave3DLayer()); }

    CircularWave3DApp(const Nodens::WindowProps props) : Application(props) { PushLayer(new CircularWave3DLayer()); }

    ~CircularWave3DApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
    return new CircularWave3DApp(Nodens::WindowProps("[NodensApp Example] CircularWave3D", 800, 600, false));
}