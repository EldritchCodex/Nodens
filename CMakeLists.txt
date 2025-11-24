cmake_minimum_required(VERSION 3.8)

# Project setup ////////////////////////////////////////////////////////////////
project(nodens VERSION 0.3.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ND_ENABLE_PROFILING "Enable profiling using Tracy" ON)


# Dependencies /////////////////////////////////////////////////////////////////
add_subdirectory(vendor)


# Engine Core //////////////////////////////////////////////////////////////////
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


# Linking //////////////////////////////////////////////////////////////////////
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


# Examples /////////////////////////////////////////////////////////////////////
add_subdirectory(examples)


# Doxygen documentation ////////////////////////////////////////////////////////
# Check if Doxygen is installed
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Define the Doxygen target
    add_custom_target(nodens-docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    # Auto-configure the Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen found. Target 'nodens-docs' created.")
else()
    message(WARNING "Doxygen not found. Documentation target will not be created.")
endif()