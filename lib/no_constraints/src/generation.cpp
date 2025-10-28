#include <generation.hpp>
#include <iostream>
#include <unordered_set>

using Cells = std::unordered_set<Cell>;

namespace {
Cells get_cell_neighbours(Cell const &cell) {
  auto const x(cell.x);
  auto const y(cell.y);
  Cells neighbours = {{x - 1, y - 1}, {x - 1, y}, {x - 1, y + 1}, {x, y + 1},
                      {x + 1, y + 1}, {x + 1, y}, {x + 1, y - 1}, {x, y - 1}};
  return neighbours;
}
} // namespace

template <typename Cells> Generation<Cells> &Generation<Cells>::next() {
  Cells cells_to_die;
  std::unordered_map<Cell, std::size_t> neighbour_count;

  for (auto const &cell : cells_alive) {
    auto nr_alive_neighbours{0};

    for (auto const &neighbour : get_cell_neighbours(cell)) {
      neighbour_count[neighbour]++;
      if (cells_alive.contains(neighbour)) {
        nr_alive_neighbours++;
      }
    }
    if ((nr_alive_neighbours != 2 && nr_alive_neighbours != 3) ||
        nr_alive_neighbours > 3) {
      // std::cout << "kill " << cell << std::endl;
      cells_to_die.insert(cell);
    }
  }

  for (auto const &cell : cells_to_die) {
    cells_alive.erase(cell);
  }

  for (auto const &[cell, count] : neighbour_count) {
    if (count == 3) {
      auto [_, inserted] = cells_alive.insert(cell);
      // inserted ? std::cout << "create " << cell << std::endl : std::cout;
    }
  }
  return *this;
}

template class Generation<Cells>;
