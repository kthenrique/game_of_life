#include <generation.hpp>
#include <gtest/gtest.h>

struct Parameters {
  Generation initial_generation;
  Generation expected_next_generation;
};

std::ostream &operator<<(std::ostream &os, const Parameters &p) {
  os << "{initial_generation=" << p.initial_generation
     << ", expected_next_generation=" << p.expected_next_generation << "}";
  return os;
}

Parameters NoNeighboursMeansExtinction = {
    Generation(Cells{{0, 0}, {0, 2}, {2, 0}, {2, 2}}), Generation()};

Parameters UnderPopulationResultsInExtinction = {
    Generation(Cells{{1, 1}, {2, 0}}), Generation()};

Parameters SurvivalBy2DoomedNeighbours = {
    Generation(Cells{{0, 2}, {1, 1}, {2, 0}}), Generation(Cells{{1, 1}})};

Parameters SurvivalBy3DoomedNeighbours = {
    Generation(Cells{{0, 0}, {1, 1}, {0, 2}, {2, 2}}),
    Generation(Cells{{0, 1}, {1, 1}, {1, 2}})};

Parameters TooManyNeighboursIsFatal = {
    Generation(Cells{{2, 0},
                     {1, 1},
                     {3, 1},
                     {0, 2},
                     {2, 2},
                     {4, 2},
                     {1, 3},
                     {3, 3},
                     {2, 4}}),
    Generation(
        Cells{{2, 0}, {1, 1}, {3, 1}, {0, 2}, {4, 2}, {1, 3}, {3, 3}, {2, 4}})};

Parameters Reproduction = {Generation(Cells{{1, 0}, {2, 0}, {0, 1}}),
                           Generation(Cells{{1, 0}, {1, 1}})};

class GoLTests : public ::testing::TestWithParam<Parameters> {};

TEST_P(GoLTests, NextGeneration) {
  auto generation = GetParam().initial_generation;
  EXPECT_EQ(generation.next(), GetParam().expected_next_generation);
}

constexpr std::array<char const *, 6> test_name = {
    "NoNeighboursMeansExtinction", "UnderPopulationResultsInExtinction",
    "SurvivalBy2DoomedNeighbours", "SurvivalBy3DoomedNeighbours",
    "TooManyNeighboursIsFatal",    "Reproduction"};

INSTANTIATE_TEST_SUITE_P(NextGeneration, GoLTests,
                         ::testing::Values(NoNeighboursMeansExtinction,
                                           UnderPopulationResultsInExtinction,
                                           SurvivalBy2DoomedNeighbours,
                                           SurvivalBy3DoomedNeighbours,
                                           TooManyNeighboursIsFatal,
                                           Reproduction),
                         [](const ::testing::TestParamInfo<Parameters> &info) {
                           return test_name[info.index];
                         });
