#include <cells.hpp>

namespace {
::boost::multiprecision::cpp_int
convert_product_to_new_grid(::boost::multiprecision::cpp_int product,
                            Cell old_grid, Cell new_grid) {
  ::boost::multiprecision::cpp_int new_product{1};
  auto const nr_factors = get_nr_factors(product);

  for (auto nth_factor{1}; nth_factor <= nr_factors; nth_factor++) {
    auto const prime = get_nth_prime_factor(nth_factor, product);
    Cell cell = get_cell_from_prime(prime, old_grid);
    new_product *= get_prime_from_cell(cell, new_grid);
  }

  return new_product;
}

Cell update_enclosing_grid(::boost::multiprecision::cpp_int product,
                           Cell old_grid) {
  Cell new_grid{0, 0};
  auto const nr_factors = get_nr_factors(product);
  for (auto nth_factor{1}; nth_factor <= nr_factors; nth_factor++) {
    auto const prime = get_nth_prime_factor(nth_factor, product);
    Cell cell = get_cell_from_prime(prime, old_grid);
    new_grid.x = new_grid.x < cell.x ? cell.x : new_grid.x;
    new_grid.y = new_grid.y < cell.y ? cell.y : new_grid.y;
  }

  return new_grid;
}

void begin_quadrant_it(Cells::Quadrant const q,
                       Cells::iterator::Quadrant_it &qi) {
  qi.enclosing_grid = q.enclosing_grid;
  qi.product = q.product;
  qi.nr_cells = get_nr_factors(q.product);
  qi.current_cell_index = 1;
}

void end_quadrant_it(Cells::Quadrant const q,
                     Cells::iterator::Quadrant_it &qi) {
  qi.enclosing_grid = q.enclosing_grid;
  qi.product = q.product;
  qi.nr_cells = get_nr_factors(q.product);
  qi.current_cell_index = qi.nr_cells + 1;
}
} // namespace

Cells::Cells(std::unordered_set<Cell> const &cells) {
  for (auto const &cell : cells) {
    Quadrant &quadrant = get_cell_quadrant(cell);
    if (std::abs(cell.x) > quadrant.enclosing_grid.x)
      quadrant.enclosing_grid.x = std::abs(cell.x);
    if (std::abs(cell.y) > quadrant.enclosing_grid.y)
      quadrant.enclosing_grid.y = std::abs(cell.y);
  }

  for (auto const &cell : cells) {
    Quadrant &quadrant = get_cell_quadrant(cell);
    auto const prime = get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                           quadrant.enclosing_grid);
    quadrant.product *= prime;
  }
}

Cells::iterator Cells::begin() const {
  iterator::Quadrant_it first{}, second{}, third{}, fourth{};
  begin_quadrant_it(m_first, first);
  begin_quadrant_it(m_second, second);
  begin_quadrant_it(m_third, third);
  begin_quadrant_it(m_fourth, fourth);

  return {first, second, third, fourth};
}

Cells::iterator Cells::end() const {
  iterator::Quadrant_it first{}, second{}, third{}, fourth{};
  end_quadrant_it(m_first, first);
  end_quadrant_it(m_second, second);
  end_quadrant_it(m_third, third);
  end_quadrant_it(m_fourth, fourth);

  return {first, second, third, fourth};
}

bool Cells::operator==(Cells const &other) const {
  return m_first == other.m_first && m_second == other.m_second &&
         m_third == other.m_third && m_fourth == other.m_fourth;
}

Cells::iterator &Cells::iterator::operator++() {

  if (first.current_cell_index <= first.nr_cells) {
    first.current_cell_index++;
    return *this;
  }
  if (second.current_cell_index <= second.nr_cells) {
    second.current_cell_index++;
    return *this;
  }
  if (third.current_cell_index <= third.nr_cells) {
    third.current_cell_index++;
    return *this;
  }
  if (fourth.current_cell_index <= fourth.nr_cells) {
    fourth.current_cell_index++;
  }

  return *this;
}

bool Cells::iterator::operator==(Cells::iterator const &other) const {
  auto const is_equal = first == other.first && second == other.second &&
                        third == other.third && fourth == other.fourth;
  return is_equal;
}

Cell Cells::iterator::operator*() const {
  if (first.current_cell_index <= first.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(first.current_cell_index, first.product),
        first.enclosing_grid);
    return cell;
  }
  if (second.current_cell_index <= second.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(second.current_cell_index, second.product),
        second.enclosing_grid);
    cell = {-1 * cell.x, cell.y};
    return cell;
  }
  if (third.current_cell_index <= third.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(third.current_cell_index, third.product),
        third.enclosing_grid);
    cell = {-1 * cell.x, -1 * cell.y};
    return cell;
  }
  if (fourth.current_cell_index <= fourth.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(fourth.current_cell_index, fourth.product),
        fourth.enclosing_grid);
    cell = {cell.x, -1 * cell.y};
    return cell;
  }
  return {};
}

bool Cells::contains(Cell cell) const {
  Quadrant const &quadrant = get_cell_quadrant(cell);
  bool is_cell_present = false;

  if (std::abs(cell.x) > quadrant.enclosing_grid.x ||
      std::abs(cell.y) > quadrant.enclosing_grid.y || quadrant.product == 1) {
    return is_cell_present;
  }
  auto const prime = get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                         quadrant.enclosing_grid);
  is_cell_present = quadrant.product % prime == 0;
  return is_cell_present;
}

std::pair<Cell, bool> Cells::insert(Cell cell) {
  Quadrant &quadrant = get_cell_quadrant(cell);

  if (contains(cell)) {
    return {cell, false};
  }

  if (quadrant.enclosing_grid.x < std::abs(cell.x) ||
      quadrant.enclosing_grid.y < std::abs(cell.y)) {
    auto const old_grid = quadrant.enclosing_grid;

    quadrant.enclosing_grid.x = quadrant.enclosing_grid.x < std::abs(cell.x)
                                    ? std::abs(cell.x)
                                    : quadrant.enclosing_grid.x;
    quadrant.enclosing_grid.y = quadrant.enclosing_grid.y < std::abs(cell.y)
                                    ? std::abs(cell.y)
                                    : quadrant.enclosing_grid.y;

    quadrant.product = convert_product_to_new_grid(quadrant.product, old_grid,
                                                   quadrant.enclosing_grid);
  }

  quadrant.product *= get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                          quadrant.enclosing_grid);

  return {cell, true};
}

void Cells::erase(Cell cell) {
  Quadrant &quadrant = get_cell_quadrant(cell);

  auto const prime = get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                         quadrant.enclosing_grid);
  quadrant.product /= prime;

  if (quadrant.product == 1) {
    quadrant.enclosing_grid = Cell{0, 0};
  } else {
    if (quadrant.enclosing_grid.x == std::abs(cell.x) ||
        quadrant.enclosing_grid.y == std::abs(cell.y)) {
      auto const old_grid = quadrant.enclosing_grid;

      quadrant.enclosing_grid =
          update_enclosing_grid(quadrant.product, quadrant.enclosing_grid);

      quadrant.product = convert_product_to_new_grid(quadrant.product, old_grid,
                                                     quadrant.enclosing_grid);
    }
  }
}

const Cells::Quadrant &Cells::get_cell_quadrant(Cell cell) const {
  return const_cast<Cells *>(this)->get_cell_quadrant(cell);
}

Cells::Quadrant &Cells::get_cell_quadrant(Cell cell) {
  if (cell.x >= 0) {
    if (cell.y >= 0) {
      return m_first;
    }
    return m_fourth;
  }

  if (cell.y >= 0) {
    return m_second;
  }
  return m_third;
}
