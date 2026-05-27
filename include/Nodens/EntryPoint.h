#pragma once

#if defined(ND_PLATFORM_WINDOWS) || defined(ND_PLATFORM_LINUX)
#pragma message("Including entry point")
extern Nodens::Application* Nodens::CreateApplication();

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Nodens::Log::Init();

    auto app = Nodens::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
#else
#error "Only Windows and Linux platforms are supported!"
#endif
