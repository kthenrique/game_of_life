add_library(GameOfLifeLib generation.cpp)

target_include_directories(GameOfLifeLib PRIVATE ${CMAKE_SOURCE_DIR})
