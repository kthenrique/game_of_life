#ifndef CELLS_HPP
#define CELLS_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <cell.hpp>
#include <prime_utils.hpp>
#include <unordered_set>

class Cells {
public:
  struct Quadrant {
    Cell enclosing_grid{0, 0};
    ::boost::multiprecision::cpp_int product{1};

    constexpr bool operator==(const Quadrant &) const = default;
  };

  // Convert default interface Cells to this Cells type
  // rationale: reuse already written tests
  Cells(std::unordered_set<Cell> const &cells);
  Cells() = default;

  struct iterator {
    struct Quadrant_it : public Quadrant {
      std::size_t nr_cells{0};
      std::size_t current_cell_index{1};
      constexpr bool operator==(const Quadrant_it &) const = default;
    } first, second, third, fourth;

    Cell operator*() const;
    iterator &operator++();
    bool operator==(iterator const &other) const;
  };

  iterator begin() const;
  iterator end() const;

  bool contains(Cell cell) const;
  std::pair<Cell, bool> insert(Cell cell);
  void erase(Cell cell);

  bool operator==(Cells const &other) const;

private:
  Quadrant m_first, m_second, m_third, m_fourth;
  Quadrant const &get_cell_quadrant(Cell cell) const;
  Quadrant &get_cell_quadrant(Cell cell);
};
#endif
