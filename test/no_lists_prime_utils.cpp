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
                        {{{0, 0}, 2}, {{1, 0}, 3}, {{0, 1}, 5}, {{1, 1}, 7}}}

        ));

TEST_P(PrimeParameterizedTests, GetPrimeWhenGridIsMinimum) {
  auto enclosing_grid = GetParam().enclosing_grid;
  auto expectations = GetParam().expectations;

  for (auto const &exp : expectations) {
    auto const &cell = exp.first;
    int const &expected_prime = exp.second;

    EXPECT_EQ(expected_prime, get_prime_from_cell(cell, enclosing_grid));
  }
}
