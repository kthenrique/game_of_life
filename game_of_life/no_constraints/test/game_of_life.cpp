#include <gtest/gtest.h>
#include <inc/generation.hpp>

TEST(GoL, NoNeighboursMeansExtinction) {

  Cells initial_cells = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};

  Generation generation{std::move(initial_cells)};

  auto next_generation = generation.next();
  Generation expected_generation;

  EXPECT_EQ(expected_generation, next_generation);
}

TEST(GoL, UnderPopulationResultsInExtinction) {

  Cells initial_cells = {{1, 1}, {2, 0}};

  Generation generation{std::move(initial_cells)};

  auto next_generation = generation.next();
  Generation expected_generation;

  EXPECT_EQ(expected_generation, next_generation);
}

TEST(GoL, SurvivalBy2DoomedNeighbours) {

  Cells initial_cells = {{0, 2}, {1, 1}, {2, 0}};
  Cells next_cells = {{1, 1}};

  Generation generation{std::move(initial_cells)};

  auto next_generation = generation.next();
  Generation expected_generation{next_cells};

  EXPECT_EQ(expected_generation, next_generation);
}

TEST(GoL, SurvivalBy3DoomedNeighbours) {

  Cells initial_cells = {{0, 0}, {1, 1}, {0, 2}, {2, 2}};
  Cells next_cells = {{1, 1}};

  Generation generation{std::move(initial_cells)};

  auto next_generation = generation.next();
  Generation expected_generation{next_cells};

  EXPECT_EQ(expected_generation, next_generation);
}

TEST(GoL, TooManyNeighboursIsFatal) {

  Cells initial_cells = {{0, 0}, {1, 0}, {0, 1}, {2, 1}, {1, 2}};
  Cells next_cells = {{0, 0}, {2, 1}, {1, 2}};

  Generation generation{std::move(initial_cells)};

  auto next_generation = generation.next();
  Generation expected_generation{next_cells};

  EXPECT_EQ(expected_generation, next_generation);
}
