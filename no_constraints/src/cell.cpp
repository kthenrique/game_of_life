#include <inc/cell.hpp>

// TODO: Remove constraint on first quadrant and allow negative cells
Cells Cell::neighbours() const {
  Cells neighbours = {{x, y + 1}, {x + 1, y + 1}, {x + 1, y}};
  auto const at_origin = x == 0 && 0 == y;
  auto const not_adjacent_to_any_axis = x != 0 && y != 0;
  if (at_origin) {
    return neighbours;
  }

  if (not_adjacent_to_any_axis) {
    neighbours.insert({{x - 1, y - 1},
                       {x - 1, y},
                       {x - 1, y + 1},
                       {x + 1, y - 1},
                       {x, y - 1}});
    return neighbours;
  }

  if (y == 0) {
    neighbours.insert({{x - 1, y}, {x - 1, y + 1}});
  }

  if (x == 0) {
    neighbours.insert({{x + 1, y - 1}, {x, y - 1}});
  }

  return neighbours;
}

std::ostream &operator<<(std::ostream &out, Cell const &cell) {
  out << "(" << cell.x << ", " << cell.y << ")";
  return out;
}
