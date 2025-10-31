#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <functional>
#include <ostream>
#include <unordered_set>

struct Cell {
  int64_t x;
  int64_t y;

  auto operator<=>(const Cell &) const = default;
};

inline std::ostream &operator<<(std::ostream &out, Cell const &cell) {
  out << "(" << cell.x << ", " << cell.y << ")";
  return out;
}

template <> struct std::hash<Cell> {
  constexpr std::size_t operator()(const Cell &c) const noexcept {
    std::size_t h1 = std::hash<int>{}(c.x);
    std::size_t h2 = std::hash<int>{}(c.y);
    // from Boost::hash_combine
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};

template <typename Cells_t>
inline Cells_t get_cell_neighbours(Cell const &cell) {
  auto const x(cell.x);
  auto const y(cell.y);
  Cells_t neighbours = std::unordered_set<Cell>{
      {x - 1, y - 1}, {x - 1, y}, {x - 1, y + 1}, {x, y + 1},
      {x + 1, y + 1}, {x + 1, y}, {x + 1, y - 1}, {x, y - 1}};
  return neighbours;
}
#endif
