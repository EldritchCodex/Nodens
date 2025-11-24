cmake_minimum_required(VERSION 3.8)
project(nodens VERSION 0.3.0 LANGUAGES CXX)

# Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(ND_ENABLE_PROFILING "Enable profiling using Tracy" ON)

# --- Dependencies ---
add_subdirectory(vendor)

# --- Engine Core ---
set(NODENS_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(NODENS_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE NODENS_SOURCE_FILES ${NODENS_SRC_PATH}/*.cpp)
file(GLOB_RECURSE NODENS_HEADER_FILES ${NODENS_INC_PATH}/*.h)

add_library(${PROJECT_NAME} STATIC
    ${NODENS_SOURCE_FILES}
    ${NODENS_HEADER_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${NODENS_SRC_PATH}
    ${NODENS_INC_PATH}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC ${NODENS_SRC_PATH}/ndpch.h)

# --- Linking ---
target_link_libraries(${PROJECT_NAME} PUBLIC
    glad
    glfw
    glm
    spdlog::spdlog
    ImGui
    ImPlot
    ImPlot3d
)

if(ND_ENABLE_PROFILING)
    target_link_libraries(${PROJECT_NAME} PUBLIC TracyClient)
endif()

# --- Examples ---
add_subdirectory(examples)