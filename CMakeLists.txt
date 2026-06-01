cmake_minimum_required(VERSION 3.30)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
project(Nodens VERSION 0.1.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Core Build Settings
# -----------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_MODULE_STD ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Developer Options (primary touchpoints)


# Default to building examples only when Nodens is the top-level project,
# not when consumed via add_subdirectory().
if(PROJECT_IS_TOP_LEVEL)
    option(ND_BUILD_EXAMPLES "Build the Nodens example applications" ON)
else()
    option(ND_BUILD_EXAMPLES "Build the Nodens example applications" OFF)
endif()

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(vendor/glad)
include(NodensDependencies)
include(NodensImGui)

# -----------------------------------------------------------------------------
# Developer Touchpoint: Framework Source Lists
# -----------------------------------------------------------------------------
set(ND_FRAMEWORK_IMPLEMENTATION_UNITS
    src/Nodens/Events/AsyncEventBus.cpp
    src/Nodens/imgui/ImGuiLayer.cpp
    src/Nodens/Application.cpp
    src/Nodens/JobSystem.cpp
    src/Nodens/Layer.cpp
    src/Nodens/LayerStack.cpp
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLImGuiRenderer.cpp
    src/Platform/GLFW/GlfwInput.cpp
    src/Platform/GLFW/GlfwWindow.cpp
)

set(ND_FRAMEWORK_MODULE_INTERFACES
    src/Nodens/Application.cppm
    src/Nodens/GraphicsContext.cppm
    src/Nodens/Input.cppm
    src/Nodens/TimeStep.cppm
    src/Nodens/JobSystem.cppm
    src/Nodens/KeyCodes.cppm
    src/Nodens/Layer.cppm
    src/Nodens/LayerStack.cppm
    src/Nodens/Log.cppm
    src/Nodens/MouseButtonCodes.cppm
    src/Nodens/imgui/ImGuiRenderer.cppm
    src/Nodens/imgui/ImGuiLayer.cppm
    src/Nodens/Window.cppm
    src/Platform/OpenGL/OpenGLContext.cppm
    src/Platform/OpenGL/OpenGLImGuiRenderer.cppm
    src/Nodens/Events/Event.cppm
    src/Nodens/Events/AsyncEventBus.cppm
)

# -----------------------------------------------------------------------------
# Framework Target
# -----------------------------------------------------------------------------
add_library(Nodens STATIC ${ND_FRAMEWORK_IMPLEMENTATION_UNITS})
add_library(Nodens::Nodens ALIAS Nodens)

target_sources(Nodens
    PUBLIC
        FILE_SET nodens_modules TYPE CXX_MODULES
        FILES ${ND_FRAMEWORK_MODULE_INTERFACES}
)

target_link_libraries(Nodens
    PUBLIC
        spdlog::spdlog
        glad
        glfw
        ImGui
        ImPlot
        ImPlot3d
        TracyClient
)

target_compile_definitions(Nodens PUBLIC
    $<$<CONFIG:Debug>:ND_DEBUG>
    $<$<CONFIG:Release>:ND_RELEASE>
)

# -----------------------------------------------------------------------------
# Optional Framework-Provided main()
# -----------------------------------------------------------------------------
add_library(NodensDefaultMain STATIC
    src/Nodens/DefaultMain.cpp
)
add_library(Nodens::DefaultMain ALIAS NodensDefaultMain)

target_link_libraries(NodensDefaultMain
    PUBLIC
        Nodens::Nodens
)

# -----------------------------------------------------------------------------
# Examples
# -----------------------------------------------------------------------------
if(ND_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------
find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    add_custom_target(nodens-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
endif()