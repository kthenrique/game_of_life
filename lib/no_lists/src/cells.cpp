#include <cells.hpp>
#include <iostream>

namespace {
std::size_t convert_product_to_new_grid(std::size_t product, Cell old_grid,
                                        Cell new_grid) {
  std::size_t new_product{1};
  auto const nr_factors = get_nr_factors(product);

  for (auto nth_factor{1}; nth_factor <= nr_factors; nth_factor++) {
    auto const prime = get_nth_prime_factor(nth_factor, product);
    Cell cell = get_cell_from_prime(prime, old_grid);
    new_product *= get_prime_from_cell(cell, new_grid);
  }

  return new_product;
}

Cell update_enclosing_grid(std::size_t product, Cell old_grid) {
  std::cout << "update_enclosing_grid: " << product << " " << old_grid
            << std::endl;
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
  std::cout << "Constructing Cells: ";
  for (auto const &cell : cells) {
    std::cout << cell << " ";
  }
  std::cout << std::endl;

  std::cout << "\tConstructing Cells: determining quadrants' enclosing grid"
            << std::endl;
  for (auto const &cell : cells) {
    Quadrant &quadrant = determine_cell_quadrant(cell);
    if (std::abs(cell.x) > quadrant.enclosing_grid.x)
      quadrant.enclosing_grid.x = std::abs(cell.x);
    if (std::abs(cell.y) > quadrant.enclosing_grid.y)
      quadrant.enclosing_grid.y = std::abs(cell.y);
  }

  std::cout << "\tConstructing Cells: calculating quadrants' products"
            << std::endl;
  for (auto const &cell : cells) {
    Quadrant &quadrant = determine_cell_quadrant(cell);
    auto const prime = get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                           quadrant.enclosing_grid);
    quadrant.product *= prime;
    std::cout << cell << ": " << prime
              << " current product: " << quadrant.product
              << " enclosing grid: " << quadrant.enclosing_grid << std::endl;
  }
  std::cout << "\tQuadrant 1: " << m_first.product << " "
            << m_first.enclosing_grid << std::endl;
  std::cout << "\tQuadrant 2: " << m_second.product << " "
            << m_second.enclosing_grid << std::endl;
  std::cout << "\tQuadrant 3: " << m_third.product << " "
            << m_third.enclosing_grid << std::endl;
  std::cout << "\tQuadrant 4: " << m_fourth.product << " "
            << m_fourth.enclosing_grid << std::endl;
  std::cout << "Cells Constructed!" << std::endl;
}

Cells::iterator Cells::begin() const {
  std::cout << "Cells::iterator begin()" << std::endl;
  iterator::Quadrant_it first{}, second{}, third{}, fourth{};
  begin_quadrant_it(m_first, first);
  begin_quadrant_it(m_second, second);
  begin_quadrant_it(m_third, third);
  begin_quadrant_it(m_fourth, fourth);

  std::cout << "\tQuadrant_it 1: " << first.product << " "
            << first.enclosing_grid << " " << first.nr_cells << " cells["
            << first.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 2: " << second.product << " "
            << second.enclosing_grid << " " << second.nr_cells << " cells["
            << second.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 3: " << third.product << " "
            << third.enclosing_grid << " " << third.nr_cells << " cells["
            << third.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 4: " << fourth.product << " "
            << fourth.enclosing_grid << " " << fourth.nr_cells << " cells["
            << fourth.current_cell_index << "]" << std::endl;
  return {first, second, third, fourth};
}

Cells::iterator Cells::end() const {
  std::cout << "Cells::iterator end()" << std::endl;
  iterator::Quadrant_it first{}, second{}, third{}, fourth{};
  end_quadrant_it(m_first, first);
  end_quadrant_it(m_second, second);
  end_quadrant_it(m_third, third);
  end_quadrant_it(m_fourth, fourth);

  std::cout << "\tQuadrant_it 1: " << first.product << " "
            << first.enclosing_grid << " " << first.nr_cells << " cells["
            << first.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 2: " << second.product << " "
            << second.enclosing_grid << " " << second.nr_cells << " cells["
            << second.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 3: " << third.product << " "
            << third.enclosing_grid << " " << third.nr_cells << " cells["
            << third.current_cell_index << "]" << std::endl;
  std::cout << "\tQuadrant_it 4: " << fourth.product << " "
            << fourth.enclosing_grid << " " << fourth.nr_cells << " cells["
            << fourth.current_cell_index << "]" << std::endl;

  return {first, second, third, fourth};
}

bool Cells::operator==(Cells const &other) const {
  return m_first == other.m_first && m_second == other.m_second &&
         m_third == other.m_third && m_fourth == other.m_fourth;
}

Cells::iterator &Cells::iterator::operator++() {
  std::cout << "Cells::iterator op++(): ";

  if (first.current_cell_index <= first.nr_cells) {
    first.current_cell_index++;
    if (first.current_cell_index != (first.nr_cells + 1)) {
      std::cout << " first quadrant - " << first.current_cell_index << "th cell"
                << std::endl;
      return *this;
    }
  }
  if (second.current_cell_index <= second.nr_cells) {
    second.current_cell_index++;
    if (second.current_cell_index != (second.nr_cells + 1)) {
      std::cout << " second quadrant - " << second.current_cell_index
                << "th cell" << std::endl;
      return *this;
    }
  }
  if (third.current_cell_index <= third.nr_cells) {
    third.current_cell_index++;
    if (third.current_cell_index != (third.nr_cells + 1)) {
      std::cout << " third quadrant - " << third.current_cell_index << "th cell"
                << std::endl;
      return *this;
    }
  }
  if (fourth.current_cell_index <= fourth.nr_cells) {
    fourth.current_cell_index++;
  }
  std::cout << " fourth quadrant - " << fourth.current_cell_index << "th cell"
            << std::endl;

  return *this;
}

bool Cells::iterator::operator==(Cells::iterator const &other) const {
  auto const is_equal = first == other.first && second == other.second &&
                        third == other.third && fourth == other.fourth;
  std::cout << "Cells::iterator op==(): " << is_equal << std::endl;
  return is_equal;
}

Cell Cells::iterator::operator*() const {
  if (first.current_cell_index <= first.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(first.current_cell_index, first.product),
        first.enclosing_grid);
    std::cout << "Cells::iterator op*() = " << cell << std::endl;
    return cell;
  }
  if (second.current_cell_index <= second.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(second.current_cell_index, second.product),
        second.enclosing_grid);
    cell = {-1 * cell.x, cell.y};
    std::cout << "Cells::iterator op*() = " << cell << std::endl;
    return cell;
  }
  if (third.current_cell_index <= third.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(third.current_cell_index, third.product),
        third.enclosing_grid);
    cell = {-1 * cell.x, -1 * cell.y};
    std::cout << "Cells::iterator op*() = " << cell << std::endl;
    return cell;
  }
  if (fourth.current_cell_index <= fourth.nr_cells) {
    auto cell = get_cell_from_prime(
        get_nth_prime_factor(fourth.current_cell_index, fourth.product),
        fourth.enclosing_grid);
    cell = {cell.x, -1 * cell.y};
    std::cout << "Cells::iterator op*() = " << cell << std::endl;
    return cell;
  }
  return {};
}

bool Cells::contains(Cell cell) {
  Quadrant &quadrant = determine_cell_quadrant(cell);
  bool is_cell_present = false;

  if (std::abs(cell.x) > quadrant.enclosing_grid.x ||
      std::abs(cell.y) > quadrant.enclosing_grid.y || quadrant.product == 1) {
    std::cout << "Cells::contain: " << cell << " " << is_cell_present
              << std::endl;
    return is_cell_present;
  }
  auto const prime = get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                         quadrant.enclosing_grid);
  is_cell_present = quadrant.product % prime == 0;
  std::cout << "Cells::contain: " << cell << " " << is_cell_present
            << std::endl;
  return is_cell_present;
}

std::pair<Cell, bool> Cells::insert(Cell cell) {
  Quadrant &quadrant = determine_cell_quadrant(cell);

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
  std::cout << "Cells::insert " << cell << " p=" << quadrant.product;

  quadrant.product *= get_prime_from_cell({std::abs(cell.x), std::abs(cell.y)},
                                          quadrant.enclosing_grid);
  std::cout << " -> p=" << quadrant.product << std::endl;

  return {cell, true};
}

void Cells::erase(Cell cell) {
  std::cout << "Cells::erase " << cell << std::endl;
  Quadrant &quadrant = determine_cell_quadrant(cell);

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
      std::cout << "\t new enclosing grid after erase: "
                << quadrant.enclosing_grid << std::endl;

      quadrant.product = convert_product_to_new_grid(quadrant.product, old_grid,
                                                     quadrant.enclosing_grid);
      std::cout << "\t new product after erase: " << quadrant.product
                << std::endl;
    }
  }
}

Cells::Quadrant &Cells::determine_cell_quadrant(Cell cell) {
  if (cell.x >= 0) {
    if (cell.y >= 0) {
      std::cout << "Cells::determine_cell_quadrant: " << cell << " => first"
                << std::endl;
      return m_first;
    }
    std::cout << "Cells::determine_cell_quadrant: " << cell << " => fourth"
              << std::endl;
    return m_fourth;
  }

  if (cell.y >= 0) {
    std::cout << "Cells::determine_cell_quadrant: " << cell << " => second"
              << std::endl;
    return m_second;
  }
  std::cout << "Cells::determine_cell_quadrant: " << cell << " => third"
            << std::endl;
  return m_third;
}
