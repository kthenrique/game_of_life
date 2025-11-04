#include <cassert>
#include <cmath>
#include <iostream>
#include <prime_utils.hpp>
#include <ranges>

namespace {
bool is_prime(std::size_t n) {
  if (n < 2)
    return false;
  for (std::size_t i = 2; i <= std::sqrt(n); ++i)
    if (n % i == 0)
      return false;
  return true;
}

std::size_t get_nth_prime(std::size_t n) {
  if (n == 0) {
    return 0;
  }
  auto primes = std::views::iota(2) | std::views::filter(is_prime);
  return *std::ranges::next(primes.begin(), n - 1);
}

} // namespace

std::size_t get_prime_from_cell(Cell const &cell, Cell const &enclosing_grid) {
  assert(cell.x <= enclosing_grid.x && cell.y <= enclosing_grid.y);

  std::size_t prime_position = (1 + enclosing_grid.x) * cell.y + cell.x + 1;
  return get_nth_prime(prime_position);
}

Cell get_cell_from_prime(std::size_t prime, Cell enclosing_grid) {
  auto nr_factors{0};
  auto nth{1};

  for (auto y{0}; y <= enclosing_grid.y; y++) {
    for (auto x{0}; x <= enclosing_grid.x; x++) {
      auto nth_prime = get_nth_prime(nth);
      if (prime == nth_prime) {
        return Cell{x, y};
      }
      nth++;
    }
  }

  assert(false);
}

std::size_t get_nr_factors(::boost::multiprecision::cpp_int product) {
  if (product <= 1) {
    return 0;
  }

  auto nr_factors{0};
  std::size_t factor{0};
  for (auto i{1}; product >= factor; ++i) {
    factor = get_nth_prime(i);
    while (product % factor == 0) {
      product /= factor;
      nr_factors++;
      if (product == 1) {
        return nr_factors;
      }
    }
  }
  assert(false);
}

std::size_t get_nth_prime_factor(std::size_t n,
                                 ::boost::multiprecision::cpp_int product) {
  auto nr_factors{0};

  for (auto i{1}; product != 1; ++i) {
    auto const prime = get_nth_prime(i);
    while (product % prime == 0) {
      product /= prime;
      nr_factors++;
      if (nr_factors == n) {
        std::cout << "  found prime = " << prime << std::endl;
        return prime;
      }
    }
  }
  assert(false);
}
