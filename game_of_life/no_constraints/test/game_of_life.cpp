#include <gtest/gtest.h>
#include <inc/generation.hpp>

TEST(GoL, UnderPopulationResultsInExtinction) {

  Cells initial_cells = {{1, 1}, {2, 0}};

  Generation generation{initial_cells};

  auto next_generation = generation.next();
  Generation expected_generation;

  EXPECT_EQ(expected_generation, next_generation);
}
