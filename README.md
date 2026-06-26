[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> ⚠️ **Development Status:** Nodens is currently in **active, experimental development** with no stable release. It serves primarily as a learning ground and playground for exploring game engine architecture and modern C++ features (specifically C++20 Modules and C++23 `std::jthread`/`std::future` concurrency). APIs are highly subject to change.

# Introduction

**Nodens** is an experimental C++23 framework designed for rapidly developing interactive desktop applications with an immediate mode graphical user interface. 

It unifies excellent third-party libraries (e.g. [ImGui](https://github.com/ocornut/imgui), [Tracy](https://github.com/wolfpld/tracy)) with a custom, multithreaded core. It provides a robust foundation for prototyping interactive applications ranging from simple visualization tools to custom game engines.

Nodens compiles into a single **static library** that is linked to your application, ensuring the final product is a single portable executable to streamline distribution.

# Documentation

| Resource | Description |
|----------|-------------|
| **[Wiki](https://github.com/EldritchCodex/Nodens/wiki)** | Tutorials, architecture guides, and how-to articles |
| **API Reference** | Doxygen-generated class and function documentation — *not yet hosted; build locally with `cmake --build build --target nodens-docs`* |

# Key Features

Nodens follows a **module-first architecture** — public APIs are C++20 module interfaces (`.cppm`), third-party headers are isolated, and consumers simply `import Nodens;`.

| Area | Highlights |
|------|-----------|
| **Core** | Layer stack system, cross-platform windowing ([GLFW](https://www.glfw.org/)), `import std;` throughout |
| **Concurrency** | `std::jthread` thread pool with `std::future` results, thread-safe Pub/Sub event bus |
| **GUI & Visualization** | [ImGui](https://github.com/ocornut/imgui) (Docking + Viewports), [ImPlot](https://github.com/epezent/implot), [ImPlot3D](https://github.com/brenocq/implot3d) |
| **Rendering** | OpenGL via [GLAD](https://glad.dav1d.de/) (Vulkan in progress) |
| **Profiling** | [Tracy](https://github.com/wolfpld/tracy) integration, [spdlog](https://github.com/gabime/spdlog) logging |

> 📖 For architecture deep-dives, see the **[Wiki](https://github.com/EldritchCodex/Nodens/wiki)**.


# Getting Started

**Requirements:** Clang 22+ (C++23 modules), CMake 3.30+, [Ninja](https://ninja-build.org/). See the **[Building and Toolchain](https://github.com/EldritchCodex/Nodens/wiki/Building-and-Toolchain)** wiki page for compatibility details.

### Using Nodens in Your Project

```cmake
include(FetchContent)

FetchContent_Declare(nodens
    GIT_REPOSITORY https://github.com/EldritchCodex/Nodens.git
    GIT_TAG        dev # Or a specific commit hash for stability
)
FetchContent_MakeAvailable(nodens)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE Nodens::Nodens)
```

### Minimal Application

```cpp
import Nodens;

class MyApp : public Nodens::Application {
public:
    static inline const Nodens::ApplicationSpecification appSpecifications = {
        .Name         = "My Nodens Application",
        .WindowWidth  = 1280,
        .WindowHeight = 720,
        .EnableGUI    = true,
        .IsHeadless   = false,
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

### Cloning & Building Locally

```shell
git clone https://github.com/EldritchCodex/Nodens.git
cd Nodens
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

> 📖 For a full walkthrough, see the **[Getting Started](https://github.com/EldritchCodex/Nodens/wiki/Getting-Started)** wiki page.

# Example Applications

Nodens includes several examples in the `examples/` directory:

| Example | Demonstrates |
|---------|-------------|
| **`circularwave3d`** | Immediate mode GUI, real-time 2D/3D plotting with ImPlot and ImPlot3D |
| **`asyncevent`** | Pub/Sub event bus, thread-safe data collection, live scatter plot visualization |

> 📖 For detailed walkthroughs of each example, see the **[Examples Guide](https://github.com/EldritchCodex/Nodens/wiki/Examples-Guide)** on the wiki.

##### Multithreaded Workload Profiling Showcase
https://github.com/user-attachments/assets/4d345eb7-46c5-4360-a7f0-55466ec753ff


# Applications Showcase

Demonstrations of the Nodens framework in action, featuring real-time interactive simulations.

https://github.com/user-attachments/assets/3025146c-d49b-4a10-84ea-3dcfa74ac59e

https://github.com/user-attachments/assets/28be4bea-d0bd-46aa-bd1c-004daedd4243


# Dependencies

All dependencies are resolved automatically at configure time via CMake `FetchContent`. Libraries with official CMake packaging (spdlog, GLFW, Tracy) are first searched on the system via `find_package`; if not found, they are downloaded and built from source. Libraries without official CMake packaging (ImGui, ImPlot, ImPlot3d) are always fetched from source.

| Library                                             | Role / Domain      | Description                                               | Version            | Resolution Strategy                 |
| :-------------------------------------------------- | :----------------- | :-------------------------------------------------------- | :----------------- | :---------------------------------- |
| **[GLAD](https://glad.dav1d.de/)**                  | **Graphics Core**  | OpenGL function loader.                                   | v0.1.35            | Generated / Vendored                |
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

[1] The Cherno (no date) Game Engine Series, YouTube. Available at: http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT.

[2] Gregory, J. (2019) *Game Engine Architecture*. Third edition. CRC Press.

[3] Horton, I. and Van Weert, P. (2023) Beginning C++23: From Beginner to Pro. Berkeley, CA: Apress. Available at: https://doi.org/10.1007/978-1-4842-9343-0.

[4] Angel, E. and Shreiner, D. (2012) Interactive computer graphics: a top-down approach with shader-based OpenGL. 6th ed. Boston: Addison-Wesley.


# License
Copyright (c) 2026 EldritchCodex.
This project and its entire history are licensed under the MIT License.
