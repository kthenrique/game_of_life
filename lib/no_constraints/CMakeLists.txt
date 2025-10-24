add_library(GoL_no_constraints src/generation.cpp src/cell.cpp)

target_link_libraries(GoL_no_constraints PUBLIC GameOfLifeIfaceLib)
