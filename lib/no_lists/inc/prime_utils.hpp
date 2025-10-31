#ifndef PRIME_UTILS_HPP
#define PRIME_UTILS_HPP

#include <cell.hpp>
#include <cstddef>

std::size_t get_prime_from_cell(Cell const &cell, Cell const &enclosing_grid);
Cell get_cell_from_prime(std::size_t prime, Cell enclosing_grid);

#endif
