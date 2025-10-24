#include <cell.hpp>

Cells Cell::neighbours() const {
  Cells neighbours = {{x - 1, y - 1}, {x - 1, y}, {x - 1, y + 1}, {x, y + 1},
                      {x + 1, y + 1}, {x + 1, y}, {x + 1, y - 1}, {x, y - 1}};
  return neighbours;
}

std::ostream &operator<<(std::ostream &out, Cell const &cell) {
  out << "(" << cell.x << ", " << cell.y << ")";
  return out;
}
