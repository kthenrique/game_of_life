#include <gtest/gtest.h>
#include <prime_utils.hpp>
#include <ranges>
#include <utility>

TEST(AssertionFailed, CellOutsideEnclosingGrid) {
  EXPECT_DEATH(get_prime_from_cell({5, 5}, {1, 1}), ",*");
}

TEST(AssertionFailed, PrimeOutsideEnclosingGrid) {
  EXPECT_DEATH(get_cell_from_prime(17, {1, 1}), ",*");
}

struct Grid_cell_prime {
  using Expected_config = std::vector<std::pair<Cell, int>>;
  Cell enclosing_grid;
  Expected_config expectations;
};

class PrimeParameterizedTests : public testing::TestWithParam<Grid_cell_prime> {
};

TEST_P(PrimeParameterizedTests, GetPrimeAndCell) {
  auto const enclosing_grid = GetParam().enclosing_grid;
  auto const expectations = GetParam().expectations;

  for (auto const &exp : expectations) {
    auto const &cell = exp.first;
    int const &prime = exp.second;

    EXPECT_EQ(prime, get_prime_from_cell(cell, enclosing_grid));
    EXPECT_EQ(cell, get_cell_from_prime(prime, enclosing_grid));
  }
}

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

TEST(NumberOfPrimeFactors, ZeroFactors) {
  EXPECT_EQ(0, get_nr_factors(0));
  EXPECT_EQ(0, get_nr_factors(1));
}

struct Factorization {
  std::size_t product;
  std::vector<std::size_t> factors;
};

class NumberOfPrimeFactors : public ::testing::TestWithParam<Factorization> {};

TEST_P(NumberOfPrimeFactors, Factors) {
  auto const product = GetParam().product;
  auto const factors = GetParam().factors;
  auto const expected_nr_factors = factors.size();

  EXPECT_EQ(expected_nr_factors, get_nr_factors(product));
  for (auto n : std::ranges::views::iota(std::size_t(0), expected_nr_factors)) {
    EXPECT_EQ(factors[n], get_nth_prime_factor(n + 1, product));
  }
}

INSTANTIATE_TEST_SUITE_P(
    PrimeFactors, NumberOfPrimeFactors,
    ::testing::Values(
        // Zero factors:
        Factorization{0, {}}, Factorization{1, {}},
        // One factor:
        Factorization{2, {2}}, Factorization{7, {7}}, Factorization{23, {23}},
        // Multiple different factors:
        Factorization{21, {3, 7}}, Factorization{110, {2, 5, 11}},
        Factorization{10465, {5, 7, 13, 23}},
        // Multiple equal factors:
        Factorization{49, {7, 7}}, Factorization{1331, {11, 11, 11}},
        Factorization{279841, {23, 23, 23, 23}}));
