#include <cells.hpp>
#include <generation.hpp>
#include <gtest/gtest.h>
#include <unordered_set>

template <typename C> struct Parameters {
  Generation<C> initial_generation;
  Generation<C> expected_next_generation;
};

std::ostream &operator<<(std::ostream &os, const Parameters<Cells> &p) {
  os << "{initial_generation=" << p.initial_generation
     << ", expected_next_generation=" << p.expected_next_generation << "}";
  return os;
}

Parameters NoNeighboursMeansExtinctionInAllQuadrants = {
    Generation(
        Cells{std::unordered_set<Cell>{{0, 0}, {0, -2}, {-2, 0}, {-2, -2}}}),
    Generation(Cells{})};

Parameters NoNeighboursMeansExtinction = {
    Generation(Cells{std::unordered_set<Cell>{{0, 0}, {0, 2}, {2, 0}, {2, 2}}}),
    Generation(Cells{})};

Parameters UnderPopulationResultsInExtinction = {
    Generation(Cells{std::unordered_set<Cell>{{1, 1}, {2, 0}}}),
    Generation(Cells{})};

Parameters SurvivalBy2DoomedNeighbours = {
    Generation(Cells{std::unordered_set<Cell>{{0, 2}, {1, 1}, {2, 0}}}),
    Generation(Cells{std::unordered_set<Cell>{{1, 1}}})};

Parameters SurvivalBy3DoomedNeighbours = {
    Generation(Cells{std::unordered_set<Cell>{{0, 0}, {1, 1}, {0, 2}, {2, 2}}}),
    Generation(Cells{std::unordered_set<Cell>{{0, 1}, {1, 1}, {1, 2}}})};

Parameters TooManyNeighboursIsFatal = {
    Generation(Cells{std::unordered_set<Cell>{{2, 0},
                                              {1, 1},
                                              {3, 1},
                                              {0, 2},
                                              {2, 2},
                                              {4, 2},
                                              {1, 3},
                                              {3, 3},
                                              {2, 4}}}),
    Generation(Cells{std::unordered_set<Cell>{
        {2, 0}, {1, 1}, {3, 1}, {0, 2}, {4, 2}, {1, 3}, {3, 3}, {2, 4}}})};

Parameters Reproduction = {
    Generation(Cells{std::unordered_set<Cell>{{1, 0}, {2, 0}, {0, 1}}}),
    Generation(Cells{std::unordered_set<Cell>{{1, 0}, {1, 1}}})};

class GoLTests : public ::testing::TestWithParam<Parameters<Cells>> {};

TEST_P(GoLTests, NextGeneration) {
  auto generation = GetParam().initial_generation;
  std::cout << "Generation: " << generation << std::endl;
  EXPECT_EQ(generation.next(), GetParam().expected_next_generation);
}

constexpr std::array<char const *, 7> test_name = {
    "NoNeighboursMeansExtinctionInAllQuadrants",
    "NoNeighboursMeansExtinction",
    "UnderPopulationResultsInExtinction",
    "SurvivalBy2DoomedNeighbours",
    "SurvivalBy3DoomedNeighbours",
    "TooManyNeighboursIsFatal",
    "Reproduction"};

INSTANTIATE_TEST_SUITE_P(
    NextGeneration, GoLTests,
    ::testing::Values(NoNeighboursMeansExtinctionInAllQuadrants,
                      NoNeighboursMeansExtinction,
                      UnderPopulationResultsInExtinction,
                      SurvivalBy2DoomedNeighbours, SurvivalBy3DoomedNeighbours,
                      TooManyNeighboursIsFatal, Reproduction),
    [](const ::testing::TestParamInfo<Parameters<Cells>> &info) {
      return test_name[info.index];
    });
