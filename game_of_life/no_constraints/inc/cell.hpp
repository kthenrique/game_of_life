#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <vector>

struct Cell {
  std::size_t x;
  std::size_t y;

  auto operator<=>(const Cell &) const = default;
};

using Cells = std::vector<Cell>;

#endif
