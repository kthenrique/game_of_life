#ifndef PRIME_UTILS_HPP
#define PRIME_UTILS_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <cell.hpp>
#include <cstddef>

std::size_t get_prime_from_cell(Cell const &cell, Cell const &enclosing_grid);
Cell get_cell_from_prime(std::size_t prime, Cell enclosing_grid);

std::size_t get_nr_factors(::boost::multiprecision::cpp_int product);
std::size_t get_nth_prime_factor(std::size_t n,
                                 ::boost::multiprecision::cpp_int product);
#endif
