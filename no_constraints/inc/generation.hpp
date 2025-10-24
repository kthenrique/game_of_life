#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <inc/cell.hpp>

class Generation {
  Cells cells_alive;

public:
  Generation(Cells inital_seed) : cells_alive{std::move(inital_seed)} {}
  Generation() = default;

  Generation &next();

  auto operator<=>(const Generation &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Generation &p);
};

std::ostream &operator<<(std::ostream &os, const Generation &p);

#endif
