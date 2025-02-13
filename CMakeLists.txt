cmake_minimum_required(VERSION 3.8)
project(nodens VERSION 0.2.0 LANGUAGES CXX)

# Options 
option(ND_ENABLE_PROFILING "Enable Tracy profiling" ON)

# Definitions

# Vendor dependencies ========================================
set(NODENS_VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# GLAD
set(GLAD_PATH ${NODENS_VENDOR_PATH}/glad)
add_subdirectory(${GLAD_PATH})

# GLFW
set(GLFW_PATH ${NODENS_VENDOR_PATH}/glfw)
add_subdirectory(${GLFW_PATH})

# imgui
set(IMGUI_PATH ${NODENS_VENDOR_PATH}/imgui)
add_subdirectory(${IMGUI_PATH})

# implot
set(IMPLOT_PATH ${NODENS_VENDOR_PATH}/implot)
add_subdirectory(${IMPLOT_PATH})

# implot3d
set(IMPLOT3D_PATH ${NODENS_VENDOR_PATH}/implot3d)
add_subdirectory(${IMPLOT3D_PATH})

# spdlog
set(SPDLOG_PATH ${NODENS_VENDOR_PATH}/spdlog)
if(NOT TARGET spdlog)
    add_subdirectory(${SPDLOG_PATH})
endif()

# Tracy
message(STATUS "Profiling: ${ND_ENABLE_PROFILING}")
set(TRACY_PATH ${NODENS_VENDOR_PATH}/tracy)
if(${ND_ENABLE_PROFILING})
    set(TRACY_ENABLE ON)
    set(TRACY_ON_DEMAND ON)
else()
    set(TRACY_ENABLE OFF)
    set(TRACY_ON_DEMAND OFF)
endif()
if(NOT TARGET tracy)
    add_subdirectory(${TRACY_PATH})
endif()

# glm (header only library)
set(GLM_PATH ${NODENS_VENDOR_PATH}/glm)
file(GLOB_RECURSE GLM_HEADER_FILES ${GLM_PATH}/glm/*.hpp)
file(GLOB_RECURSE GLM_INLINE_FILES ${GLM_PATH}/glm/*.inl)

# Nodens Library ============================================
set(NODENS_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(NODENS_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE NODENS_SOURCE_FILES ${NODENS_SRC_PATH}/*.cpp)
file(GLOB_RECURSE NODENS_HEADER_FILES ${NODENS_INC_PATH}/*.h)

add_library(${PROJECT_NAME} STATIC
    ${NODENS_SOURCE_FILES}
    ${NODENS_HEADER_FILES}
    ${GLM_HEADER_FILES}
    ${GLM_INLINE_FILES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    $<$<CONFIG:Debug>:ND_DEBUG>
    $<$<CONFIG:Release>:ND_RELEASE>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${NODENS_SRC_PATH}
    ${NODENS_INCLUDE_PATH}
    ${NODENS_VENDOR_PATH}
    ${IMGUI_PATH}
    ${IMPLOT_PATH}
    ${IMPLOT3D_PATH}
    ${GLM_PATH}
    ${GLAD_PATH}/include
    ${GLFW_PATH}/include
    ${SPDLOG_PATH}/include
    ${TRACY_PATH}/public/tracy
)

# precompiled headers
target_precompile_headers(${PROJECT_NAME}
    PUBLIC
    ${NODENS_SRC_PATH}/ndpch.h
)

# link opengl lib
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    glad
    imgui
    implot
    implot3d
    glfw
    spdlog
    TracyClient
)

# Tell GLFW not to include OpenGL headers (already included by glad)
add_definitions(-DGLFW_INCLUDE_NONE)
