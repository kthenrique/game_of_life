add_library(GameOfLifeLib generation.cpp cell.cpp)

target_include_directories(GameOfLifeLib PRIVATE ${CMAKE_SOURCE_DIR})
