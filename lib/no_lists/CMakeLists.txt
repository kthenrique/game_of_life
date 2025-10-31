add_library(GoL_no_lists src/cells.cpp src/prime_utils.cpp)
target_include_directories(GoL_no_lists PRIVATE inc/)
target_link_libraries(GoL_no_lists PUBLIC GameOfLifeIfaceLib)
