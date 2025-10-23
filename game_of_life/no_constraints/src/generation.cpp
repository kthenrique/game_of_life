#include <inc/generation.hpp>

Generation &Generation::next() {
  cells_alive = Cells();

  return *this;
}
