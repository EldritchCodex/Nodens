cmake_minimum_required(VERSION 3.30)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
project(Nodens VERSION 0.1.0 LANGUAGES CXX)
# -----------------------------------------------------------------------------
# Core Build Settings
# -----------------------------------------------------------------------------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER_IMPORT_STD ON)
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

find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    target_sources(ImGui PRIVATE
        "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
    )
    target_link_libraries(ImGui PUBLIC Vulkan::Vulkan)
endif()

set(ND_VULKAN_MODULES)
if(Vulkan_FOUND)
    if(NOT TARGET Vulkan::cppm)
        add_library(NodensVulkanCppModule STATIC)
        add_library(Vulkan::cppm ALIAS NodensVulkanCppModule)

        target_compile_definitions(NodensVulkanCppModule PUBLIC
            VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
            VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
            VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING=1
        )
        target_include_directories(NodensVulkanCppModule PRIVATE
            "${Vulkan_INCLUDE_DIR}"
        )
        target_link_libraries(NodensVulkanCppModule PUBLIC Vulkan::Vulkan)
        set_target_properties(NodensVulkanCppModule PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
        )
        target_sources(NodensVulkanCppModule PUBLIC
            FILE_SET vulkan_modules TYPE CXX_MODULES
            BASE_DIRS "${Vulkan_INCLUDE_DIR}"
            FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(NodensVulkanCppModule PRIVATE
                -Wno-reserved-module-identifier
                -Wno-include-angled-in-module-purview
            )
        endif()
    endif()

    list(APPEND ND_VULKAN_MODULES src/Platform/Vulkan/VulkanContext.cppm)
endif()

# -----------------------------------------------------------------------------
# Developer Touchpoint: Framework Source Lists
# -----------------------------------------------------------------------------
set(ND_FRAMEWORK_IMPLEMENTATION_UNITS
    # Core
    src/Nodens/Core/Application.cpp
    # Layers
    src/Nodens/Layers/Layer.cpp
    src/Nodens/Layers/LayerStack.cpp
    # Events
    src/Nodens/Events/EventBus.cpp
    # Concurrency
    src/Nodens/Concurrency/JobSystem.cpp
    # ImGui
    src/Nodens/ImGui/ImGuiLayer.cpp
    src/Nodens/ImGui/DefaultTheme.cpp
    # Platform
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLImGuiRenderer.cpp
    src/Platform/GLFW/GlfwInput.cpp
    src/Platform/GLFW/GlfwWindow.cpp
)

set(ND_FRAMEWORK_MODULE_INTERFACES
    # Core
    src/Nodens/Core/Application.cppm
    src/Nodens/Core/Log.cppm
    src/Nodens/Core/TimeStep.cppm
    # Layers
    src/Nodens/Layers/Layer.cppm
    src/Nodens/Layers/LayerStack.cppm
    # Events
    src/Nodens/Events/Event.cppm
    src/Nodens/Events/EventBus.cppm
    src/Nodens/Events/InputEvents.cppm
    # Concurrency
    src/Nodens/Concurrency/JobSystem.cppm
    # ImGui
    src/Nodens/ImGui/ImGuiRenderer.cppm
    src/Nodens/ImGui/ImGuiLayer.cppm
    src/Nodens/ImGui/DefaultTheme.cppm
    # Input
    src/Nodens/Input/Input.cppm
    src/Nodens/Input/KeyCodes.cppm
    src/Nodens/Input/MouseButtonCodes.cppm
    # Rendering
    src/Nodens/Rendering/GraphicsContext.cppm
    src/Nodens/Rendering/Window.cppm
    # Platform
    src/Platform/OpenGL/OpenGLContext.cppm
    src/Platform/OpenGL/OpenGLImGuiRenderer.cppm
    # Optional Vulkan platform context
    ${ND_VULKAN_MODULES}
    # Umbrella module
    src/Nodens/nodens.cppm
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

target_compile_features(Nodens PUBLIC cxx_std_23)

target_compile_definitions(Nodens PUBLIC
    $<$<CONFIG:Debug>:ND_DEBUG>
    $<$<CONFIG:Release>:ND_RELEASE>
    SPDLOG_USE_STD_FORMAT
)

if(Vulkan_FOUND)
    target_compile_definitions(Nodens PRIVATE ND_HAS_VULKAN)
    target_link_libraries(Nodens PUBLIC Vulkan::cppm)
endif()

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
