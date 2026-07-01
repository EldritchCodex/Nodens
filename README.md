[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> ⚠️ **Development Status:** Nodens is currently in **active, experimental development** with no stable release. It serves primarily as a learning ground and playground for exploring game engine architecture and modern C++ features (specifically C++20 modules, concepts for metaprogramming, `std::jthread`/`std::future` for concurrency; and C++23 `import std;`, `std::move_only_function`, `std::to_underlying`, etc). APIs are highly subject to change.

# Introduction

**Nodens** is an experimental C++23 framework designed for rapidly developing interactive desktop applications with an optional immediate mode graphical user interface. 

It unifies excellent third-party libraries (e.g. [ImGui](https://github.com/ocornut/imgui), [ImPlot](https://github.com/epezent/implot), [ImPlot3D](https://github.com/brenocq/implot3d), [Tracy](https://github.com/wolfpld/tracy)) with a custom core that integrates 3 main systems:
- **Layer System** that governs the application lifetime and execution flow, visual rendering priority and input event routing (see [wiki](https://github.com/EldritchCodex/Nodens/wiki/Layer-System));
- **Event System** with InputEvent routing through the LayerStack and a pub/sub thread-safe EventBus for both immediate and queued dispatch of custom event types (see [wiki](https://github.com/EldritchCodex/Nodens/wiki/Event-System));
- **Job System** for multithreaded task execution (see [wiki](https://github.com/EldritchCodex/Nodens/wiki/Concurrency-and-JobSystem)).

Nodens follows a **module-first architecture** where public APIs are C++20 module interfaces (`.cppm`), third-party headers are isolated on the global fragment module, and consumers simply `import Nodens;`.

The framework compiles into a single **static library** that is linked to your application, ensuring the final product is a single portable executable to streamline distribution.

# Documentation

| Resource | Description |
|----------|-------------|
| **[Wiki](https://github.com/EldritchCodex/Nodens/wiki)** | Tutorials, architecture guides, and how-to articles |
| **API Reference** | Doxygen-generated class and function documentation - *not yet hosted; build locally with `cmake --build build --target nodens-docs`* |

# Quick Start

> 📖 For a full walkthrough, see the **[Getting Started](https://github.com/EldritchCodex/Nodens/wiki/Getting-Started)** wiki page.

**Requirements:** Clang 22+, CMake 3.30+, Ninja 1.13.2. See the **[Building and Toolchain](https://github.com/EldritchCodex/Nodens/wiki/Building-and-Toolchain)** wiki page for compatibility details.

### Minimal Application Using Nodens

```cmake
# CMakeLists.txt
include(FetchContent)

FetchContent_Declare(nodens
    GIT_REPOSITORY https://github.com/EldritchCodex/Nodens.git
    GIT_TAG        dev # Or a specific commit hash for stability
)
FetchContent_MakeAvailable(nodens)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE Nodens::Nodens)
```

```cpp
// main.cpp
import Nodens;

class MyApp : public Nodens::Application {
public:
    static inline const Nodens::ApplicationSpecification appSpecifications = {
        .Name         = "My Nodens Application",
        .WindowWidth  = 1280,
        .WindowHeight = 720,
        .EnableGUI    = true,
        .IsHeadless   = false,        
        .ShouldImGuiBlockInputs = true,
        .DefaultTheme = Nodens::EDefaultTheme::Dark
    };

    MyApp() : Application(appSpecifications) {
        // Add layers here, e.g., PushLayer(new MyLayer());
    }
};

int main()
{
    Nodens::InitializeLoggers();
    auto app = MyApp();
    app.Run();
    
    return 0;
}
```

# Cloning & Building Locally

```shell
git clone https://github.com/EldritchCodex/Nodens.git
cd Nodens
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

# Example Applications

Nodens includes several examples in the `examples/` directory:

| Example | Demonstrates |
|---------|-------------|
| **`circularwave3d`** | Immediate mode GUI, real-time 2D/3D plotting with ImPlot and ImPlot3D |
<<<<<<< Updated upstream
| **`asyncevent`** | Pub/Sub event bus, thread-safe data collection, live scatter plot visualization |
=======
| **`planetaryscan`** | Pub/Sub event bus, multi-threaded job system with thread-safe data collection, live scatter plot visualization |
>>>>>>> Stashed changes

> 📖 For detailed walkthroughs of each example, see the **[Examples Guide](https://github.com/EldritchCodex/Nodens/wiki/Examples-Guide)** on the wiki.

##### Multithreaded Workload Profiling Showcase
https://github.com/user-attachments/assets/4d345eb7-46c5-4360-a7f0-55466ec753ff


# Other Applications Showcase

Applications I used in my own research on numerical simulations using earlier versions of Nodens.

https://github.com/user-attachments/assets/3025146c-d49b-4a10-84ea-3dcfa74ac59e

https://github.com/user-attachments/assets/28be4bea-d0bd-46aa-bd1c-004daedd4243


# Dependencies

All dependencies are resolved automatically at configure time via CMake `FetchContent` and are downloaded and built from source. 

| Library                                             | Role / Domain      | Description                                               | Version            | Resolution Strategy                 |
| :-------------------------------------------------- | :----------------- | :-------------------------------------------------------- | :----------------- | :---------------------------------- |
| **[GLAD](https://glad.dav1d.de/)**                  | **Graphics Core**  | OpenGL function loader.                                   | v0.1.35            | Generated from the website                |
| **[GLFW](https://www.glfw.org/)**                   | **System & Input** | Cross-platform for window, context, and input management. | 3.4                | FetchContent + `find_package`       |
| **[ImGui](https://github.com/ocornut/imgui)**       | **UI / Tools**     | Bloat-free Immediate Mode GUI.                            | v1.92.8-docking    | FetchContent (always from source)   |
| **[ImPlot](https://github.com/epezent/implot)**     | **Visualization**  | 2D data plotting extension for ImGui.                     | v1.0               | FetchContent (always from source)   |
| **[ImPlot3D](https://github.com/brenocq/implot3d)** | **Visualization**  | 3D data plotting extension for ImGui.                     | v0.4               | FetchContent (always from source)   |
| **[spdlog](https://github.com/gabime/spdlog)**      | **Utilities**      | Fast, header-only/compiled logging library.               | v1.17.0            | FetchContent + `find_package`       |
| **[Tracy](https://github.com/wolfpld/tracy)**       | **Profiling**      | Real-time frame profiler.                                 | v0.13.1            | FetchContent + `find_package`       |

# Acknowledgements

A special thanks to **TheCherno**. The foundational architecture of Nodens was heavily inspired by the early episodes of his excellent [Game Engine series](http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) on YouTube.

Additionally, Nodens originally began as a personal project for my undergraduate Computer Graphics course at UFMG. Early iterations of this framework powered several of my academic projects, including [boids](https://github.com/renanbomtempo/boids) and [galaxians](https://github.com/renanbomtempo/galaxians).

# References

- Gregory, J. (2019) *Game Engine Architecture*. Third edition. CRC Press.
- Horton, I. and Van Weert, P. (2023) Beginning C++23: From Beginner to Pro. Berkeley, CA: Apress. Available at: https://doi.org/10.1007/978-1-4842-9343-0.
- The Cherno (no date) Game Engine Series, YouTube. Available at: http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT.

# License
Copyright (c) 2026 EldritchCodex.
This project and its entire history are licensed under the MIT License.
