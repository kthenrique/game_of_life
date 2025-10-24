add_library(GoL_no_lists src/generation.cpp src/cell.cpp)
target_link_libraries(GoL_no_lists PUBLIC GameOfLifeIfaceLib)
