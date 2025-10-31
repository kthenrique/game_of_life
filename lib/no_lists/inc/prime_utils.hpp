#ifndef PRIME_UTILS_HPP
#define PRIME_UTILS_HPP

#include <cell.hpp>
#include <cstddef>

std::size_t get_prime_from_cell(Cell const &cell, Cell const &enclosing_grid);
Cell get_cell_from_prime(std::size_t prime, Cell enclosing_grid);

std::size_t get_nr_factors(std::size_t product);
std::size_t get_nth_prime_factor(std::size_t n, std::size_t product);
#endif
