#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <inc/cell.hpp>

class Generation {
  Cells cells_alive;

public:
  Generation(Cells inital_seed) : cells_alive{inital_seed} {}
  Generation() = default;

  Generation &next() {
    cells_alive = Cells();

    return *this;
  }

  auto operator<=>(const Generation &) const = default;
};

#endif
