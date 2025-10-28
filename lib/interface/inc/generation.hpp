#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <cell.hpp>

template <typename Cells> class Generation {
  Cells cells_alive;

public:
  Generation(Cells inital_seed) : cells_alive{std::move(inital_seed)} {}
  Generation() = default;

  Generation &next();
  Cells const &get_alive_cells() const { return cells_alive; };

  auto operator<=>(const Generation &) const = default;

  template <typename Cells_>
  friend std::ostream &operator<<(std::ostream &os,
                                  const Generation<Cells_> &p);
};

template <typename Cells>
inline std::ostream &operator<<(std::ostream &os,
                                const Generation<Cells> &gen) {
  os << "[ ";
  for (auto const &cell : gen.cells_alive) {
    os << cell << " ";
  }
  os << "]";

  return os;
}

#endif
