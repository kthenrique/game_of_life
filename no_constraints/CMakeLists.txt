cmake_minimum_required(VERSION 3.28.3)

project(GameOfLife_noConstraints VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Main Lib
add_subdirectory(src)

# GameOfLife
add_executable(GameOfLife)
target_sources(GameOfLife
    PRIVATE
    src/main.cpp
)
target_include_directories(GameOfLife
    PRIVATE
    inc/
)
target_link_libraries(GameOfLife PRIVATE GameOfLifeLib)

# Tests
enable_testing()
add_subdirectory(test)
