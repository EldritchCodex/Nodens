#pragma once

#ifdef ND_PLATFORM_WINDOWS
#pragma message("Including entry point")
extern Nodens::Application* Nodens::CreateApplication();
int main(int argc, char** argv) {
  Nodens::Log::Init();

  auto app = Nodens::CreateApplication();
  app->Run();
  delete app;
}
#else
#error Nodens currently only supports Windows!
#endif
