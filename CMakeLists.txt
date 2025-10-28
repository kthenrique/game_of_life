cmake_minimum_required(VERSION 3.28.3)

project(GameOfLife VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IMPLEMENTATION "Choose library implementation")

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(viewer)

if(IMPLEMENTATION STREQUAL "no_constraints")
    add_library(GameOfLifeLib ALIAS GoL_no_constraints)
elseif(IMPLEMENTATION STREQUAL "no_lists")
    add_library(GameOfLifeLib ALIAS GoL_no_lists)
else()
    message(FATAL_ERROR "Unknown IMPLEMENTATION: ${IMPLEMENTATION}")
endif()

# Tests
enable_testing()
add_subdirectory(test)
