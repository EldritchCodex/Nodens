cmake_minimum_required(VERSION 3.26)
project(asyncevent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ND_ENABLE_PROFILING ON)

if(NOT TARGET Nodens::Nodens)
    find_package(Nodens REQUIRED)
endif()

add_executable(asyncevent
    AsyncEvent.h
    AsyncEventApp.cpp
    AsyncEventLayer.cpp
    AsyncEventLayer.h
)

target_link_libraries(asyncevent PRIVATE Nodens::Nodens)