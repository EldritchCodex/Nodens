cmake_minimum_required(VERSION 3.8)
project(sinewave3d LANGUAGES CXX)

# Set Nodens options
set(ND_ENABLE_PROFILING ON)

# Definitions ====================================
if(WIN32)
    add_definitions(-DND_PLATFORM_WINDOWS)
endif()

# Nodens Source files
set(NODENS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Sinewave Source files
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${NODENS_DIR}/include)

# Link nodens lib
link_directories()
target_link_libraries(${PROJECT_NAME} PRIVATE nodens)