![nodens-banner3](https://github.com/user-attachments/assets/1a0e2932-e99d-4432-8e26-88a350368cd7)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Introduction

**Nodens** aims to be a simple, C++23 framework designed for quickly developing interactive desktop applications with an immediate mode graphical user interface.

Built upon C++23 standards, the framework unifies amazing third-party libraries, like [ImGui](https://github.com/ocornut/imgui) and [Tracy](https://github.com/wolfpld/tracy), with a custom, multithreaded core, providing a robust foundation for rapid development/prototyping of interactive applications ranging from simple tools to game engines.

The framework compiles into a single **static library** that is linked to the application to generate a **single executable file to ensure applications remain portable** and to **streamline distribution.**


# Architecture

Nodens now follows a **module-first architecture** across the framework and examples.

- Public interfaces are authored as C++ module interface units (`.cppm`) using `export module`.
- Implementations live in module implementation units (`.cpp`) using `module ...`.
- Internal framework APIs are consumed through explicit imports (e.g. `import Nodens.Application;`) instead of local header includes.
- C++ standard library usage in module units is based on `import std;`.
- Third-party ecosystems (GLFW, ImGui, ImPlot, Tracy, etc.) remain include-based where needed, typically isolated in module global fragments or implementation units.

This keeps ownership boundaries explicit and makes dependencies easier to reason about as the codebase grows.


# Key Features

### 🖥️ Core Architecture
- **C++23 + Modules:** Built with C++23 features (e.g. `std::jthread`, `std::stop_token`, concepts and `std::to_underlying`) and a module-first design (`export module`, `import std;`, `import Nodens.*`).
- **Layer Stack System:** Flexible application flow control allowing for modular updates and rendering layers (e.g., overlay, game world, UI).
- **Window Management:** cross-platform windowing and input polling via [GLFW](https://www.glfw.org/).

### ⚡ Concurrency & Events
- **Multithreaded Job System:** A custom thread pool implementation utilizing C++20 `std::jthread` for automatic joining and `std::future` for asynchronous task management.
- **Asynchronous Event Bus:** A thread-safe Publish/Subscribe system allowing decoupled communication between subsystems. Supports generic event types and lambda listeners.

### 🎨 Graphics & GUI
- **Immediate Mode GUI:** Fully integrated [ImGui](https://github.com/ocornut/imgui) with Docking and Viewport support enabled by default.
- **Data Visualization:** Native support for high-performance 2D and 3D plotting via [ImPlot](https://github.com/epezent/implot) and [ImPlot3D](https://github.com/brenocq/implot3d).
- **Rendering Backend:** OpenGL context management initialized via [GLAD](https://glad.dav1d.de/).

### 🛠️ Profiling & Debugging
- **Integrated Frame Profiling:** Built-in support for [Tracy Profiler](https://github.com/wolfpld/tracy) to analyze frame time, memory usage, and lock contention in real-time.
- **Logging:** Integrated [SPDLog](https://github.com/gabime/spdlog) logging system.


# Getting Started

### Prerequisites
* **C++ Compiler:** A compiler with strong C++23 modules support (development is being done with Clang 22.1.6).
* **CMake:** Version **3.30** or higher.

### Toolchain Notes (Modules)
- Nodens uses CMake module file sets (`FILE_SET ... TYPE CXX_MODULES`) for exported module interfaces.
- Build configuration enables C++ standard library module imports (`import std;`) through CMake's module settings.
- If you use an older or partially supported compiler/CMake combination, module discovery or BMI generation may fail during configuration/build.

### Cloning
To clone the repository run the command
```shell
git clone https://github.com/EldritchCodex/Nodens.git --recursive
```

### Using Nodens Modules in Applications
Applications consume Nodens through module imports. A typical entry unit imports framework modules directly:

```cpp
import Nodens.Application;
import Nodens.Log;
```

Example applications in `examples/` follow the same model and also define their own local modules via `.cppm` files.

### Optional Framework-Provided Entry Point
If you prefer not to write a `main()` for simple example applications, Nodens provides an optional framework-supplied entry point. Link your executable against the `Nodens::DefaultMain` target and omit your own `main()` implementation. The default `main()` performs the minimal, correct startup sequence:

- Calls `Nodens::InitializeLogging()`
- Calls the application factory `Nodens::CreateApplication()` (must be implemented by the application)
- Runs the application (`Application::Run()`)
- Destroys the application instance and returns

Usage (CMake):

```cmake
target_link_libraries(yourapp PRIVATE Nodens::DefaultMain)
```

Notes:
- If your executable defines `main()` already, keep it — do not link `Nodens::DefaultMain` to avoid duplicate `main` symbols.
- The `Nodens::CreateApplication()` factory must be present in your code (examples define this in their module interface units).

### Example Applications
Examples of application codes are provided in the `examples/` folder.

#### `circularwave3d`

- **Demonstrates:** Immediate Mode GUI Rendering, 2D/3D Plotting, Profiling.
- **Description:** A visual demo that renders real-time mathematical functions. It calculates sine and cosine waves every frame and visualizes them using **ImPlot** (for 2D graphs) and **ImPlot3D** (for 3D line plots).

#### `jobsystem`

- **Demonstrates:** Multithreading, `std::future` integration, Non-blocking UI.
- **Description:** A control panel for the internal thread pool. It allows the user to submit a "Heavy Calculation" (simulated by a 2-second thread sleep) to a background worker. The main thread polls the `std::future` status each frame to check for completion without freezing the GUI, updating the status text from "Processing..." to "Idle" once finished.

#### `asyncevent`

- **Demonstrates:** Pub/Sub Architecture, Thread-safe Data Gathering, Live Analytics.
- **Description:** A simulation of a "Deep Space Analytics" tool. It uses the event bus to decouple the UI from the logic:
   - **Publisher:** Buttons on the UI publish `PlanetaryScanEvent`s.
   - **Subscriber:** A background worker listens for these events, simulates a randomized workload (variable sleep time), generates "scientific data" (distance/atmosphere density), and reports the results back.
   - **Visualization:** The main thread safely locks the data mutex to render the incoming results on a scatter plot and a latency graph in real-time.

##### Showcasing performance profiling for multithreaded workloads

https://github.com/user-attachments/assets/4d345eb7-46c5-4360-a7f0-55466ec753ff


# Applications Showcase

Demonstrations of the Nodens framework in action, featuring real-time interactive simulations.

https://github.com/user-attachments/assets/3025146c-d49b-4a10-84ea-3dcfa74ac59e

https://github.com/user-attachments/assets/28be4bea-d0bd-46aa-bd1c-004daedd4243


# Dependencies

| Library                                             | Role / Domain      | Description                                               | Integration Method   | Version/Branch/Tag |
| :-------------------------------------------------- | :----------------- | :-------------------------------------------------------- | :------------------- | :----------------- |
| **[GLAD](https://glad.dav1d.de/)**                  | **Graphics Core**  | OpenGL function loader.                                   | Generated / External | v0.1.35            |
| **[GLFW](https://www.glfw.org/)**                   | **System & Input** | Cross-platform for window, context, and input management. | **Git Submodule**    | v3.4               |
| **[ImGui](https://github.com/ocornut/imgui)**       | **UI / Tools**     | Bloat-free Immediate Mode GUI.                            | **Git Submodule**    | v1.92.8-docking    |
| **[ImPlot](https://github.com/epezent/implot)**     | **Visualization**  | 2D data plotting extension for ImGui.                     | **Git Submodule**    | v1.0               |
| **[ImPlot3D](https://github.com/brenocq/implot3d)** | **Visualization**  | 3D data plotting extension for ImGui.                     | **Git Submodule**    | v0.4               |
| **[SPDLog](https://github.com/gabime/spdlog)**      | **Utilities**      | Fast, header-only/compiled logging library.               | **Git Submodule**    | v1.17.0            |
| **[Tracy](https://github.com/wolfpld/tracy)**       | **Profiling**      | Real-time frame profiler.                                 | **Git Submodule**    | v0.13.0            |

# References

[1] The Cherno (no date) Game Engine Series, YouTube. Available at: http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT.

[2] Gregory, J. (2019) *Game Engine Architecture*. Third edition. CRC Press.

[3] Horton, I. and Van Weert, P. (2023) Beginning C++23: From Beginner to Pro. Berkeley, CA: Apress. Available at: https://doi.org/10.1007/978-1-4842-9343-0.

[4] Angel, E. and Shreiner, D. (2012) Interactive computer graphics: a top-down approach with shader-based OpenGL. 6th ed. Boston: Addison-Wesley.

# License
Copyright (c) 2026 EldritchCodex.
This project and its entire history are licensed under the MIT License.
