#include <algorithm>
#include <inc/generation.hpp>
#include <iostream>
#include <map>

Generation &Generation::next() {
  Cells cells_to_die;
  Cells all_neighbours;

  for (auto const &cell : cells_alive) {
    auto nr_alive_neighbours{0};
    auto neighbours = cell.neighbours();
    all_neighbours.insert(all_neighbours.end(), neighbours.begin(),
                          neighbours.end());

    for (auto const &neighbour : neighbours) {
      if (std::ranges::find(cells_alive, neighbour) != cells_alive.end()) {
        nr_alive_neighbours++;
      }
    }
    if ((nr_alive_neighbours != 2 && nr_alive_neighbours != 3) ||
        nr_alive_neighbours > 3) {
      cells_to_die.push_back(cell);
    }
  }

  for (auto const &cell : cells_to_die) {
    std::erase(cells_alive, cell);
  }

  std::map<Cell, int> neighbour_counts;
  for (auto const &cell : all_neighbours) {
    neighbour_counts[cell]++;
  }

  for (auto const &[cell, count] : neighbour_counts) {
    if (count == 3) {
      if (std::ranges::find(cells_alive, cell) == cells_alive.end()) {
        cells_alive.push_back(cell);
      }
    }
  }
  return *this;
}
