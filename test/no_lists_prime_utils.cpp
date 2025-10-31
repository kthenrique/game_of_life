#include <array>
#include <gtest/gtest.h>
#include <prime_utils.hpp>
#include <utility>

struct Grid_cell_prime {
  using Expected_config = std::vector<std::pair<Cell, int>>;
  Cell enclosing_grid;
  Expected_config expectations;
};

class PrimeParameterizedTests : public testing::TestWithParam<Grid_cell_prime> {
};

INSTANTIATE_TEST_SUITE_P(
    PrimeUtils, PrimeParameterizedTests,
    ::testing::Values(
        // GRID 1x1:
        // |2|
        Grid_cell_prime{{0, 0}, {{{0, 0}, 2}}},
        // GRID 2x2:
        // |5|7|
        // |2|3|
        Grid_cell_prime{{1, 1},
                        {{{0, 0}, 2}, {{1, 0}, 3}, {{0, 1}, 5}, {{1, 1}, 7}}},
        // GRID 1x2:
        // |3|
        // |2|
        Grid_cell_prime{{0, 1}, {{{0, 0}, 2}, {{0, 1}, 3}}},
        // GRID 3x2:
        // | 7|11|13|
        // | 2| 3| 5|
        Grid_cell_prime{{2, 1},
                        {{{0, 0}, 2},
                         {{1, 0}, 3},
                         {{2, 0}, 5},
                         {{0, 1}, 7},
                         {{0, 1}, 7},
                         {{1, 1}, 11},
                         {{2, 1}, 13}}}

        ));

TEST_P(PrimeParameterizedTests, GetPrimeWhenGridIsMinimum) {
  auto enclosing_grid = GetParam().enclosing_grid;
  auto expectations = GetParam().expectations;

  for (auto const &exp : expectations) {
    auto const &cell = exp.first;
    int const &prime = exp.second;

    EXPECT_EQ(prime, get_prime_from_cell(cell, enclosing_grid));
    EXPECT_EQ(cell, get_cell_from_prime(prime, enclosing_grid));
  }
}

TEST(AssertionFailed, CellOutsideEnclosingGrid) {
  EXPECT_DEATH(get_prime_from_cell({5, 5}, {1, 1}), ",*");
}

TEST(AssertionFailed, PrimeOutsideEnclosingGrid) {
  EXPECT_DEATH(get_cell_from_prime(17, {1, 1}), ",*");
}
