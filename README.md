![nodens-banner3](https://github.com/user-attachments/assets/1a0e2932-e99d-4432-8e26-88a350368cd7)

# Introduction

**Nodens** is a C++ framework for interactive desktop applications (Windows only for now).

It combines third-party libraries with a core custom framework to provide:
 - user input processing and window management (from [GLFW](https://www.glfw.org/));
 - an immediate mode GUI (from [ImGui](https://github.com/ocornut/imgui));
 - 2D/3D graph plotting (from [ImPlot](https://github.com/epezent/implot) and [ImPlot3D](https://github.com/brenocq/implot3d));
 - frame profiling (from [Tracy](https://github.com/wolfpld/tracy) 0.11.1).
 - multithreaded job system (custom thread pool using `std::future` for background tasks);
 - asynchronous event bus (thread-safe pub/sub system using the `JobSystem`);

# Applications Showcase
Here are some applications created using the Nodens framework.

https://github.com/user-attachments/assets/3025146c-d49b-4a10-84ea-3dcfa74ac59e

https://github.com/user-attachments/assets/28be4bea-d0bd-46aa-bd1c-004daedd4243

# References

[1] Gregory, J. (2019) Game engine architecture. Third edition. Boca Raton London New York: CRC Press, Taylor & Francis Group (An A.K. Peters book).

[2] The Cherno (no date) Game Engine Series, YouTube. Available at: http://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT.
