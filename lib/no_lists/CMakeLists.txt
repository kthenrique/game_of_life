include(FetchContent)

FetchContent_Declare(
    boost_multiprecision
    GIT_REPOSITORY https://github.com/boostorg/multiprecision.git
    GIT_TAG Boost_1_89_0
)

FetchContent_MakeAvailable(boost_multiprecision)

add_library(GoL_no_lists src/cells.cpp src/prime_utils.cpp)
target_include_directories(
    GoL_no_lists
    PRIVATE inc/
    PUBLIC ${boost_multiprecision_SOURCE_DIR}/include/
)

target_compile_definitions(GoL_no_lists PRIVATE BOOST_MP_STANDALONE)

target_link_libraries(GoL_no_lists PUBLIC GameOfLifeIfaceLib)
