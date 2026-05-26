cmake_minimum_required(VERSION 3.26)
project(Nodens VERSION 0.1.0 LANGUAGES CXX)


# Setup ////////////////////////////////////////////////////////////////////////
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Options //////////////////////////////////////////////////////////////////////
option(ND_ENABLE_PROFILING "Enable profiling using Tracy" ON)


# Dependencies /////////////////////////////////////////////////////////////////
add_subdirectory(vendor)


# Engine Core //////////////////////////////////////////////////////////////////
add_library(Nodens STATIC
    src/Nodens/Events/ApplicationEvent.h
    src/Nodens/Events/AsyncEventBus.cpp
    src/Nodens/Events/AsyncEventBus.h
    src/Nodens/Events/Event.h
    src/Nodens/Events/KeyEvent.h
    src/Nodens/Events/MouseEvent.h

    src/Nodens/imgui/ImGuiLayer.cpp
    src/Nodens/imgui/ImGuiLayer.h
    src/Nodens/imgui/ImGuiRenderer.h

    src/Nodens/Application.cpp
    src/Nodens/Application.h
    src/Nodens/Core.h
    src/Nodens/EntryPoint.h
    src/Nodens/GraphicsContext.h
    src/Nodens/Input.h
    src/Nodens/JobSystem.cpp
    src/Nodens/JobSystem.h
    src/Nodens/KeyCodes.h
    src/Nodens/Layer.cpp
    src/Nodens/Layer.h
    src/Nodens/LayerStack.cpp
    src/Nodens/LayerStack.h
    src/Nodens/Log.cpp
    src/Nodens/Log.h
    src/Nodens/MouseButtonCodes.h
    src/Nodens/TimeStep.h
    src/Nodens/Window.h

    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLContext.h
    src/Platform/OpenGL/OpenGLImGuiRenderer.cpp
    src/Platform/OpenGL/OpenGLImGuiRenderer.h

    src/Platform/Windows/WindowsInput.cpp
    src/Platform/Windows/WindowsInput.h
    src/Platform/Windows/WindowsWindow.cpp
    src/Platform/Windows/WindowsWindow.h
)


# Include Directories & Precompiled Headers ////////////////////////////////////
target_include_directories(Nodens PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_precompile_headers(Nodens PUBLIC src/ndpch.h)


# Alias ////////////////////////////////////////////////////////////////////////
add_library(Nodens::Nodens ALIAS Nodens)


# Linking //////////////////////////////////////////////////////////////////////
target_link_libraries(Nodens PUBLIC
    glad
    glfw
    glm
    spdlog::spdlog
    ImGui
    ImPlot
    ImPlot3d
)

target_compile_definitions(Nodens PUBLIC
    $<$<CONFIG:Debug>:ND_DEBUG>
    $<$<CONFIG:Release>:ND_RELEASE>
    $<$<BOOL:${WIN32}>:ND_PLATFORM_WINDOWS>
)

if(ND_ENABLE_PROFILING)
    target_link_libraries(Nodens PUBLIC TracyClient)
endif()


# Examples /////////////////////////////////////////////////////////////////////
add_subdirectory(examples)


# Doxygen documentation ////////////////////////////////////////////////////////
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Define the Doxygen target
    add_custom_target(nodens-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

    # Auto-configure the Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen found. Target 'nodens-docs' created.")
else()
    message(WARNING "Doxygen not found. Documentation target will not be created.")
endif()