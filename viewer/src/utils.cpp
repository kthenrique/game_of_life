#include <theme.hpp>
#include <utils.hpp>

#include <ftxui/dom/elements.hpp>

namespace utils {
ftxui::Element get_header() {
  auto title = ftxui::vbox({
                   ftxui::text("┏━╸┏━┓┏┳┓┏━╸ ┏━┓┏━╸ ╻  ╻┏━╸┏━╸"),
                   ftxui::text("┃╺┓┣━┫┃┃┃┣╸  ┃ ┃┣╸  ┃  ┃┣╸ ┣╸ "),
                   ftxui::text("┗━┛╹ ╹╹ ╹┗━╸ ┗━┛╹   ┗━╸╹╹  ┗━╸"),

               }) |
               ftxui::bold | theme::main_color | ftxui::hcenter;

  auto author_line =
      ftxui::text("by Kelve T. Henrique") | ftxui::dim | ftxui::align_right;

  auto header = ftxui::vbox({
      title,
      author_line,
  });
  return header;
}

ftxui::Element dead_cell() {
  return ftxui::text("□") | ftxui::center |
         size(ftxui::WIDTH, ftxui::EQUAL, 2) |
         size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

ftxui::Element alive_cell() {
  return ftxui::text("■") | ftxui::center | theme::cell_color |
         size(ftxui::WIDTH, ftxui::EQUAL, 2) |
         size(ftxui::HEIGHT, ftxui::EQUAL, 1);
}

} // namespace utils
