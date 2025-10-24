#include <algorithm>
#include <inc/generation.hpp>

Generation &Generation::next() {
  Cells cells_to_die;
  std::unordered_map<Cell, std::size_t> neighbour_count;

  for (auto const &cell : cells_alive) {
    auto nr_alive_neighbours{0};
    auto neighbours = cell.neighbours();

    for (auto const &neighbour : neighbours) {
      neighbour_count[neighbour]++;
      if (std::ranges::find(cells_alive, neighbour) != cells_alive.end()) {
        nr_alive_neighbours++;
      }
    }
    if ((nr_alive_neighbours != 2 && nr_alive_neighbours != 3) ||
        nr_alive_neighbours > 3) {
      cells_to_die.insert(cell);
    }
  }

  for (auto const &cell : cells_to_die) {
    cells_alive.erase(cell);
  }

  for (auto const &[cell, count] : neighbour_count) {
    if (count == 3) {
      cells_alive.insert(cell);
    }
  }
  return *this;
}
