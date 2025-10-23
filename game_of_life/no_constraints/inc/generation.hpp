#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <inc/cell.hpp>

class Generation {
  Cells cells_alive;

public:
  Generation(Cells cells) : cells_alive{cells} {}
  Generation() = default;

  Generation &next() { return *this; }

  auto operator<=>(const Generation &) const = default;
};

#endif
