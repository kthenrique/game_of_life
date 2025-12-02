#include <cassert>
#include <cmath>
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

std::size_t get_prime_position(std::size_t upper_bound) {
  if (upper_bound < 2) {
    return 0;
  }
  auto count{1};
  std::size_t prime{3};
  for (std::size_t p = 3; p <= upper_bound; p += 2) {
    if (is_prime(p)) {
      ++count;
    }
  }
  return count;
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
  const std::size_t nth_prime = get_prime_position(prime);

  const std::size_t width = enclosing_grid.x + 1;
  const std::size_t height = enclosing_grid.y + 1;
  const std::size_t total = width * height;

  if (nth_prime == 0 || nth_prime > total) {
    std::ostringstream oss;
    oss << "Prime " << prime << " (nth=" << nth_prime
        << ") does not fit in grid " << enclosing_grid;
    throw std::out_of_range(oss.str());
  }

  const std::size_t idx = nth_prime - 1;
  const std::int64_t y = idx / width;
  const std::int64_t x = idx % width;

  return Cell{x, y};
}

std::size_t get_nr_factors(::boost::multiprecision::cpp_int product) {
  if (product <= 1) {
    return 0;
  }

  auto nr_factors{0};
  while (product % 2 == 0) {
    product /= 2;
    ++nr_factors;
  }

  boost::multiprecision::cpp_int limit = sqrt(product);
  std::size_t factor{3};
  while (factor <= limit) {
    if (product % factor == 0) {
      do {
        product /= factor;
        nr_factors++;
      } while (product % factor == 0);
      limit = sqrt(product);
    }
    factor += 2;
  }
  if (product != 1) {
    nr_factors++;
  }

  return nr_factors;
}

std::size_t get_nth_prime_factor(std::size_t n,
                                 ::boost::multiprecision::cpp_int product) {
  if (product <= 1 || n == 0) {
    throw std::invalid_argument("n must be >= 1 and product > 1");
  }

  auto nr_factors{0};
  while (product % 2 == 0) {
    product /= 2;
    if (++nr_factors == n) {
      return 2;
    }
  }

  boost::multiprecision::cpp_int limit = sqrt(product);
  std::size_t factor{3};
  while (nr_factors != n && factor <= limit) {
    if (product % factor == 0) {
      do {
        product /= factor;
        if (++nr_factors == n) {
          return factor;
        }
      } while (product % factor == 0);
      limit = sqrt(product);
    }
    factor += 2;
  }

  if (product != 1) {
    if (++nr_factors == n) {
      if (product > std::numeric_limits<std::size_t>::max()) {
        throw std::overflow_error("prime factor does not fit in size_t");
      }
      return static_cast<std::size_t>(product);
    }
  }

  throw std::out_of_range("product has fewer than n prime factors");
}
