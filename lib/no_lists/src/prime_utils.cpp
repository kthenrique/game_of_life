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
  std::size_t prime_position = (1 + enclosing_grid.x) * cell.y + cell.x + 1;
  std::cout << "get_prime_from_cell: ie. " << prime_position << "th prime"
            << std::endl;
  return get_nth_prime(prime_position);
}
