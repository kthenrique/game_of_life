#include <generation.hpp>
#include <iostream>

Generation &Generation::next() {
  Cells cells_to_die;
  std::unordered_map<Cell, std::size_t> neighbour_count;

  for (auto const &cell : cells_alive) {
    auto nr_alive_neighbours{0};

    for (auto const &neighbour : cell.neighbours()) {
      neighbour_count[neighbour]++;
      if (cells_alive.contains(neighbour)) {
        nr_alive_neighbours++;
      }
    }
    if ((nr_alive_neighbours != 2 && nr_alive_neighbours != 3) ||
        nr_alive_neighbours > 3) {
      std::cout << "kill " << cell << std::endl;
      cells_to_die.insert(cell);
    }
  }

  for (auto const &cell : cells_to_die) {
    cells_alive.erase(cell);
  }

  for (auto const &[cell, count] : neighbour_count) {
    if (count == 3) {
      auto [_, inserted] = cells_alive.insert(cell);
      inserted ? std::cout << "create " << cell << std::endl : std::cout;
    }
  }
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Generation &gen) {
  os << "[ ";
  for (auto const &cell : gen.cells_alive) {
    os << cell << " ";
  }
  os << "]";

  return os;
}
