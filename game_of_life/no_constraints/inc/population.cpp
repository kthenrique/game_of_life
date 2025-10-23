#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <inc/generation.hpp>

class Population {
  Generation m_current_generation;

public:
  Population(Generation initial_seed) : m_current_generation{initial_seed} {}

  Population() = default;
};

#endif
