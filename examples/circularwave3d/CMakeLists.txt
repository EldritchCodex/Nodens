cmake_minimum_required(VERSION 3.26)
project(circularwave3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ND_ENABLE_PROFILING ON)

if(NOT TARGET Nodens::Nodens)
    find_package(Nodens REQUIRED)
endif()

add_executable(circularwave3d
    CircularWave3DApp.cpp
    CircularWave3DLayer.cpp
    CircularWave3DLayer.h
)

target_link_libraries(circularwave3d PRIVATE Nodens::Nodens)