# Standard CMake Libraries /////////////////////////////////////////////////////

# GLAD
add_subdirectory(glad)

# GLFW
add_subdirectory(glfw)

# Spdlog
add_subdirectory(spdlog)

# Tracy
if(ND_ENABLE_PROFILING)
    add_subdirectory(tracy)
endif()

# GLM (Header Only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_RADIANS GLM_FORCE_DEPTH_ZERO_TO_ONE)


# Manual Builds ////////////////////////////////////////////////////////////////
# --- ImGui ---
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

add_library(ImGui STATIC
# Core
${IMGUI_DIR}/imgui.cpp
${IMGUI_DIR}/imgui_draw.cpp
${IMGUI_DIR}/imgui_tables.cpp
${IMGUI_DIR}/imgui_widgets.cpp

# Backends
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Define include paths explicitly to ensure they are found
target_include_directories(ImGui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    "${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include"  # <--- Explicitly add GLFW include
    "${CMAKE_CURRENT_SOURCE_DIR}/glad/include"  # <--- Explicitly add GLAD include
)
target_compile_definitions(ImGui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GLFW_INCLUDE_NONE)
target_link_libraries(ImGui PUBLIC glfw glad)


# --- ImPlot ---
set(IMPLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/implot")
add_library(ImPlot STATIC
${IMPLOT_DIR}/implot.cpp
${IMPLOT_DIR}/implot_items.cpp
)
target_include_directories(ImPlot PUBLIC ${IMPLOT_DIR})
target_link_libraries(ImPlot PUBLIC ImGui)

# --- ImPlot3d ---
set(IMPLOT3D_DIR "${CMAKE_CURRENT_SOURCE_DIR}/implot3d")
add_library(ImPlot3d STATIC
${IMPLOT3D_DIR}/implot3d.cpp
${IMPLOT3D_DIR}/implot3d_items.cpp
)
target_include_directories(ImPlot3d PUBLIC ${IMPLOT3D_DIR})
target_link_libraries(ImPlot3d PUBLIC ImGui)