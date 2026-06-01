![nodens-banner3](https://github.com/user-attachments/assets/1a0e2932-e99d-4432-8e26-88a350368cd7)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> ⚠️ **Development Status:** Nodens is currently in **active, experimental development** with no stable release. It serves primarily as a learning ground and playground for exploring game engine architecture and modern C++23 features (specifically C++23 Modules and `std::jthread`/`std::future` concurrency). APIs are highly subject to change.

# Introduction

**Nodens** is an experimental C++23 framework designed for rapidly developing interactive desktop applications with an immediate mode graphical user interface. 

It unifies excellent third-party libraries (e.g. [ImGui](https://github.com/ocornut/imgui), [Tracy](https://github.com/wolfpld/tracy)) with a custom, multithreaded core. It provides a robust foundation for prototyping interactive applications ranging from simple visualization tools to custom game engines.

Nodens compiles into a single **static library** that is linked to your application, ensuring the final product is a single portable executable to streamline distribution.

# Architecture & Key Features

### Core Architecture
Nodens strictly follows a **module-first architecture** across both the framework core and its examples. This keeps ownership boundaries explicit and makes dependencies easier to reason about as the codebase grows:
- **C++20 Modules:** Public interfaces are authored as C++ module interface units (`.cppm`) using `export module`, and implementations live in `.cpp` units. Internal framework APIs are consumed through explicit imports (e.g., `import Nodens.Application;`).
- **C++23 Standard Library:** C++ standard library usage heavily relies on `import std;`.
- **Dependency Isolation:** Third-party ecosystems (GLFW, ImGui, ImPlot, Tracy, etc.) remain `#include`-based where necessary, typically isolated within module global fragments or internal implementation units to avoid leaking legacy headers to consumers.
- **Layer Stack System:** Flexible application flow control allowing for modular updates and rendering layers (e.g., UI overlay, game world rendering).
- **Window Management:** Cross-platform windowing and input polling powered by [GLFW](https://www.glfw.org/).

### Concurrency & Events
- **Multithreaded Job System:** A custom thread pool implementation utilizing C++20 `std::jthread` for automatic joining and `std::future` for asynchronous task management.
- **Asynchronous Event Bus:** A thread-safe Publish/Subscribe system allowing decoupled communication between subsystems. Supports generic event types and lambda listeners.

### Graphics & GUI
- **Immediate Mode GUI:** Fully integrated [ImGui](https://github.com/ocornut/imgui) with Docking and Viewports enabled by default.
- **Data Visualization:** Native support for high-performance 2D and 3D plotting via [ImPlot](https://github.com/epezent/implot) and [ImPlot3D](https://github.com/brenocq/implot3d).
- **Rendering Backend:** Currently using OpenGL context management initialized via [GLAD](https://glad.dav1d.de/). A Vulkan backend is currently being developed.

### Profiling & Debugging
- **Integrated Frame Profiling:** Deep integration with [Tracy Profiler](https://github.com/wolfpld/tracy) to analyze frame times, memory usage, and lock contention in real-time.
- **Logging:** Fast, color-coded console logging using [spdlog](https://github.com/gabime/spdlog).


# Getting Started

### Prerequisites
* **C++ Compiler:** A compiler with robust C++23 modules support (Currently developed and tested with Clang 22.1.6).
* **CMake:** Version **3.30** or higher.
* **Build Generator:** [Ninja](https://ninja-build.org/) is **required**. Standard Unix Makefiles do not natively support C++20 module dependency scanning.

### Toolchain Notes (Modules)
- Nodens uses CMake module file sets (`FILE_SET ... TYPE CXX_MODULES`) for exported module interfaces.
- The build configuration forcefully enables C++ standard library module imports (`import std;`) through CMake's experimental/native module settings.
- If you use an older or partially supported compiler/CMake combination, module discovery or Built Module Interface (BMI) generation will fail during configuration or build.

---

### Using Nodens in Your Project

Because Nodens does not yet have stable releases, the recommended way to integrate it is by pointing CMake's `FetchContent` directly to the `dev` branch.

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

> **Note on Dependencies:** Dependencies like spdlog, GLFW, and Tracy are resolved transparently. If they are already installed on your system (e.g. via `vcpkg`, `Conan`, or `apt`), those installations are preferred via `find_package`. Otherwise, they are downloaded and built from source automatically alongside Nodens.

#### Writing a Nodens Application

Applications consume Nodens through module imports. A typical entry unit imports framework modules directly rather than including headers:

```cpp
import Nodens.Application;
import Nodens.Log;

class MyApp : public Nodens::Application {
   // ...
};

namespace Nodens {
   Application* CreateApplication() {
      return new MyApp({...});
   }
}

int main()
{
   Nodens::InitializeLogging();

   auto app = Nodens::CreateApplication();
   app->Run();
   delete app;

   return 0;
}
```

#### Optional Framework-Provided Entry Point
If you prefer not to write a `main()` function for simple applications, Nodens provides an optional framework-supplied entry point. Link your executable against the `Nodens::DefaultMain` target and omit your own `main()`. 

The default `main()` performs a correct, minimal startup sequence:
1. Initializes the core logging system.
2. Calls the application factory `Nodens::CreateApplication()` (which **you** must implement).
3. Runs the application loop (`Application::Run()`).
4. Safely destroys the application instance and returns.

```cmake
target_link_libraries(myapp PRIVATE Nodens::DefaultMain)
```

---

### Cloning & Building Locally

If you want to contribute to Nodens or run the examples locally:

```shell
git clone https://github.com/EldritchCodex/Nodens.git
cd Nodens
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

**CMake Options:**
| Option | Default | Description |
|--------|---------|-------------|
| `ND_BUILD_EXAMPLES` | `ON` (if top-level) / `OFF` (if consumed) | Build the example applications located in `examples/`. Automatically disabled when Nodens is consumed as a dependency via `FetchContent`. |


# Example Applications

Nodens includes several examples in the `examples/` directory that demonstrate how to use the framework's architecture.

#### `circularwave3d`
- **Demonstrates:** Immediate Mode GUI Rendering, 2D/3D Plotting, Profiling.
- **Description:** A visual demo that renders real-time mathematical functions. It calculates sine and cosine waves every frame and visualizes them using **ImPlot** (for 2D graphs) and **ImPlot3D** (for 3D line plots).

#### `jobsystem`
- **Demonstrates:** Multithreading, `std::future` integration, Non-blocking UI.
- **Description:** A control panel for the internal thread pool. It allows the user to submit a "Heavy Calculation" (simulated by a 2-second thread sleep) to a background worker. The main thread polls the `std::future` status each frame to check for completion without freezing the GUI, updating the status text from "Processing..." to "Idle" once finished.

#### `asyncevent`
- **Demonstrates:** Pub/Sub Architecture, Thread-safe Data Gathering, Live Analytics.
- **Description:** A simulation of a "Deep Space Analytics" tool. It uses the event bus to decouple the UI from the logic:
   - **Publisher:** UI buttons publish `PlanetaryScanEvent`s.
   - **Subscriber:** A background worker listens for these events, simulates a randomized workload, generates "scientific data" (distance/atmosphere density), and reports the results back.
   - **Visualization:** The main thread safely locks the data mutex to render incoming results on a scatter plot and a latency graph in real-time.

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
