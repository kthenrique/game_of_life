cmake_minimum_required(VERSION 3.28.3)

project(GameOfLife VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IMPLEMENTATION "Choose library implementation")

add_subdirectory(lib)

set(_known_impls "no_constraints" "no_lists")

if(NOT IMPLEMENTATION IN_LIST _known_impls)
    message(
        WARNING
        "Unknown implementation: ${IMPLEMENTATION}. Defaulting to no_constraints."
    )
    set(IMPLEMENTATION "no_constraints")
endif()

# Create alias to always point to the chosen impl
set(_lib_target "GoL_${IMPLEMENTATION}")
add_library(GameOfLifeLib ALIAS ${_lib_target})
message(STATUS "Using implementation: ${IMPLEMENTATION} -> ${_lib_target}")

add_subdirectory(app)
add_subdirectory(viewer)

# Tests
enable_testing()
add_subdirectory(test)
