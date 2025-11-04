#ifndef UTILS_HPP
#define UTILS_HPP

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>

namespace utils {

enum Runtime_ctrl : uint8_t {
  rewind = 0,
  start = 1,
  pause = 2,
  stop = 3,
  fast_forward = 4,

};

ftxui::Element get_header();

ftxui::Element dead_cell();
ftxui::Element alive_cell();

} // namespace utils
#endif
