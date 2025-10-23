add_library(GameOfLifeLib game_of_life.cpp)

target_include_directories(GameOfLifeLib PRIVATE ${CMAKE_SOURCE_DIR})
