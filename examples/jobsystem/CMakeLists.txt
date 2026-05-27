cmake_minimum_required(VERSION 3.26)
project(example-jobsystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ND_ENABLE_PROFILING ON)

if(NOT TARGET Nodens::Nodens)
    find_package(Nodens REQUIRED)
endif()

add_executable(example-jobsystem
    JobSystemApp.cpp
    JobSystemLayer.cpp
    JobSystemLayer.h
)

target_link_libraries(example-jobsystem PRIVATE Nodens::Nodens)