include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(GameOfLifeTests game_of_life.cpp)
target_link_libraries(GameOfLifeTests PRIVATE GameOfLifeLib GTest::gtest_main)
target_include_directories(GameOfLifeTests PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/${IMPLEMENTATION}/inc/)

#gtest_discover_tests(GameOfLifeTests)
add_test(NAME GameOfLifeTests COMMAND GameOfLifeTests)
