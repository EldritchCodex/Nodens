![nodens-banner3](https://github.com/user-attachments/assets/1a0e2932-e99d-4432-8e26-88a350368cd7)

# Introduction

**Nodens** is a simple, modern C++ framework designed for quickly developing interactive desktop applications with a immediate mode graphical user interface.

Built upon **C++23** standards, the framework unifies amazing third-party libraries, like [ImGui](https://github.com/ocornut/imgui) and [Tracy](https://github.com/wolfpld/tracy), with a custom, multithreaded core, providing a robust foundation for rapid development/prototyping of interactive applications ranging from simple tools to game engines.


# Key Features

### 🖥️ Core Architecture
* **Modern C++ Standard:** Built using C++20/23 features (e.g. `std::jthread`, `std::stop_token`, concepts and `std::to_underlying`).
* **Layer Stack System:** Flexible application flow control allowing for modular updates and rendering layers (e.g., overlay, game world, UI).
* **Window Management:** cross-platform windowing and input polling via [GLFW](https://www.glfw.org/).

### ⚡ Concurrency & Events
* **Multithreaded Job System:** A custom thread pool implementation utilizing C++20 `std::jthread` for automatic joining and `std::future` for asynchronous task management.
* **Asynchronous Event Bus:** A thread-safe Publish/Subscribe system allowing decoupled communication between subsystems. Supports generic event types and lambda listeners.

### 🎨 Graphics & GUI
* **Immediate Mode GUI:** Fully integrated [ImGui](https://github.com/ocornut/imgui) with Docking and Viewport support enabled by default.
* **Data Visualization:** Native support for high-performance 2D and 3D plotting via [ImPlot](https://github.com/epezent/implot) and [ImPlot3D](https://github.com/brenocq/implot3d).
* **Rendering Backend:** OpenGL context management initialized via [GLAD](https://glad.dav1d.de/).

### 🛠️ Profiling & Debugging
* **Integrated Frame Profiling:** Built-in support for [Tracy Profiler](https://github.com/wolfpld/tracy) (v0.13.0) to analyze frame time, memory usage, and lock contention in real-time.


# Getting Started

### Prerequisites
* **C++ Compiler:** A compiler with C++23 support.
* **CMake:** Version 3.8 or higher.

### Cloning
To clone the repository run the command
```shell
git clone https://github.com/EldritchCodex/Nodens.git --recursive
```

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


# Applications Showcase

Demonstrations of the Nodens framework in action, featuring real-time interactive simulations.

https://github.com/user-attachments/assets/3025146c-d49b-4a10-84ea-3dcfa74ac59e

https://github.com/user-attachments/assets/28be4bea-d0bd-46aa-bd1c-004daedd4243


# References

### Dependencies

| Library                                             | Role / Domain      | Description                                               | Integration Method   | Version/Branch/Tag |
| :-------------------------------------------------- | :----------------- | :-------------------------------------------------------- | :------------------- | :----------------- |
| **[GLAD](https://glad.dav1d.de/)**                  | **Graphics Core**  | OpenGL function loader.                                   | Generated / External | 0.1.35             |
| **[GLFW](https://www.glfw.org/)**                   | **System & Input** | Cross-platform for window, context, and input management. | **Git Submodule**    | 3.4                |
| **[GLM](https://github.com/g-truc/glm)**            | **Mathematics**    | Header-only mathematics library.                          | **Git Submodule**    | 1.0.2              |
| **[ImGui](https://github.com/ocornut/imgui)**       | **UI / Tools**     | Bloat-free Immediate Mode GUI.                            | **Git Submodule**    | v1.92.5-docking    |
| **[ImPlot](https://github.com/epezent/implot)**     | **Visualization**  | 2D data plotting extension for ImGui.                     | **Git Submodule**    | master             |
| **[ImPlot3D](https://github.com/brenocq/implot3d)** | **Visualization**  | 3D data plotting extension for ImGui.                     | **Git Submodule**    | main               |
| **[SPDLog](https://github.com/gabime/spdlog)**      | **Utilities**      | Fast, header-only/compiled logging library.               | **Git Submodule**    | v1.16.0            |
| **[Tracy](https://github.com/wolfpld/tracy)**       | **Profiling**      | Real-time frame profiler.                                 | **Git Submodule**    | v0.13.0            |

### Literature

[1] The Cherno (no date) Game Engine Series, YouTube. Available at: http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT.

[2] Gregory, J. (2019) *Game Engine Architecture*. Third edition. CRC Press.

[3] Horton, I. and Van Weert, P. (2023) Beginning C++23: From Beginner to Pro. Berkeley, CA: Apress. Available at: https://doi.org/10.1007/978-1-4842-9343-0.

[4] Angel, E. and Shreiner, D. (2012) Interactive computer graphics: a top-down approach with shader-based OpenGL. 6th ed. Boston: Addison-Wesley.
