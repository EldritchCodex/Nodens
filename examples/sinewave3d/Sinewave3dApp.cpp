/*	Set this file as the Startup Item for CMakeLists.txt
 */
#define MAIN_APPLICATION_FILE

#include "Sinewave3dLayer.h"
#include "nodens.h"

// Main application layer
class Sinewave3dApp : public Nodens::Application {
 public:
  Sinewave3dApp() { PushLayer(new Sinewave3dLayer()); }

  Sinewave3dApp(const Nodens::WindowProps props) : Application(props) {
    PushLayer(new Sinewave3dLayer());
  }

  ~Sinewave3dApp() {}
};

Nodens::Application* Nodens::CreateApplication() {
  return new Sinewave3dApp(
      Nodens::WindowProps("[NodensApp Example] Sinewave3d", 800, 600, false));
}