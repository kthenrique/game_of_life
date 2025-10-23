#include <inc/generation.hpp>
#include <iostream>

Generation &Generation::next() {
  Cells cells_to_die;

  for (auto const &cell : cells_alive) {
    auto nr_alive_neighbours{0};
    auto neighbours = cell.neighbours();

    for (auto const &neighbour : neighbours) {
      if (std::ranges::find(cells_alive, neighbour) != cells_alive.end()) {
        nr_alive_neighbours++;
      }
    }
    if (nr_alive_neighbours != 2) {
      cells_to_die.push_back(cell);
    }
  }

  for (auto const &cell : cells_to_die) {
    std::erase(cells_alive, cell);
  }
  return *this;
}
