/*	Set this file as the Startup Item for CMakeLists.txt
 */

#define MAIN_APPLICATION_FILE
#include "NodensTestLayer.h"
#include "nodens.h"

// Main application layer
class NodensTestApp : public Nodens::Application {
 public:
  NodensTestApp(const Nodens::WindowProps props) : Application(props) {
    PushLayer(new NodensTestLayer());
  }

  ~NodensTestApp() {}
};

Nodens::Application *Nodens::CreateApplication() {
  return new NodensTestApp(
      Nodens::WindowProps("Nodens Test App", 800, 600, false));
}