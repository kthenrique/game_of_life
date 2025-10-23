#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <inc/cell.hpp>
#include <vector>

class Generation {
  std::vector<Cell> alive;

public:
  Generation() = default;
};
#endif
