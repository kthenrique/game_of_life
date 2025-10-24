#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <functional>
#include <unordered_set>

struct Cell;

using Cells = std::unordered_set<Cell>;

struct Cell {
  std::size_t x;
  std::size_t y;

  auto operator<=>(const Cell &) const = default;
  Cells neighbours() const;
};

template <> struct std::hash<Cell> {
  constexpr std::size_t operator()(const Cell &c) const noexcept {
    // (from Boost::hash_combine)
    std::size_t h1 = std::hash<int>{}(c.x);
    std::size_t h2 = std::hash<int>{}(c.y);
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};
#endif
