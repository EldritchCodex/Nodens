cmake_minimum_required(VERSION 3.26)
project(Nodens VERSION 0.1.0 LANGUAGES CXX)


# Setup ////////////////////////////////////////////////////////////////////////
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Options //////////////////////////////////////////////////////////////////////
option(ND_ENABLE_PROFILING "Enable profiling using Tracy" ON)
option(ND_BUILD_EXAMPLES "Build the Nodens example applications" ON)


# Dependencies /////////////////////////////////////////////////////////////////
add_subdirectory(vendor)


# Engine Core //////////////////////////////////////////////////////////////////
add_library(Nodens STATIC
    src/Nodens/Events/AsyncEventBus.cpp

    src/Nodens/imgui/ImGuiLayer.cpp
    src/Nodens/imgui/ImGuiLayer.h
    src/Nodens/imgui/ImGuiRenderer.h

    src/Nodens/Application.cpp
    src/Nodens/GraphicsContext.h
    src/Nodens/JobSystem.cpp
    src/Nodens/Layer.cpp
    src/Nodens/LayerStack.cpp
    src/Nodens/LayerStack.h
    src/Nodens/Log.cpp

    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLContext.h
    src/Platform/OpenGL/OpenGLImGuiRenderer.cpp
    src/Platform/OpenGL/OpenGLImGuiRenderer.h

    src/Platform/Windows/WindowsInput.cpp
    src/Platform/Windows/WindowsInput.h
    src/Platform/Windows/WindowsWindow.cpp
    src/Platform/Windows/WindowsWindow.h

    include/Nodens.h
    include/Nodens/Application.h
    include/Nodens/Core.h
    include/Nodens/EntryPoint.h
    include/Nodens/Input.h
    include/Nodens/JobSystem.h
    include/Nodens/KeyCodes.h
    include/Nodens/Layer.h
    include/Nodens/Log.h
    include/Nodens/MouseButtonCodes.h
    include/Nodens/TimeStep.h
    include/Nodens/Window.h
    include/Nodens/Events/ApplicationEvent.h
    include/Nodens/Events/AsyncEventBus.h
    include/Nodens/Events/Event.h
    include/Nodens/Events/KeyEvent.h
    include/Nodens/Events/MouseEvent.h
)


# Include Directories & Precompiled Headers ////////////////////////////////////
target_include_directories(Nodens
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_precompile_headers(Nodens PRIVATE src/ndpch.h)


# Alias ////////////////////////////////////////////////////////////////////////
add_library(Nodens::Nodens ALIAS Nodens)


# Linking //////////////////////////////////////////////////////////////////////
target_link_libraries(Nodens
    PUBLIC
        spdlog::spdlog
        glad
        glfw
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
if(ND_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


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