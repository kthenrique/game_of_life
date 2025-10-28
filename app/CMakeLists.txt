add_executable(GameOfLife
    main.cpp
)

target_include_directories(GameOfLife PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/${IMPLEMENTATION}/inc/)

target_link_libraries(GameOfLife PRIVATE GameOfLifeLib)
