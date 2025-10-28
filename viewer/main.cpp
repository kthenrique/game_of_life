#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
using namespace ftxui;

void bcb() { std::cout << "Button clicked!\n"; }

int main() {
  constexpr auto alive{"■"};
  constexpr auto dead{"□"};

  auto screen = ScreenInteractive::Fullscreen();

  std::vector<Elements> rows;
  for (int y = 0; y < 21; ++y) {
    std::vector<Element> row;
    for (int x = 0; x < 21; ++x) {
      if (y % 7 == 0 && x % 12 == 0)
        row.push_back(text(alive) | center);
      else
        row.push_back(text(dead) | center);
    }
    rows.push_back(std::move(row) | size(WIDTH, EQUAL, 2) |
                   size(HEIGHT, EQUAL, 1));
  }

  auto grid_element = gridbox(std::move(rows)) | vcenter | hcenter;

  auto grid_component = Renderer([&] { return grid_element; });

  // Components (buttons)
  auto up_btn = Button("⏶", bcb) | hcenter;
  auto left_btn = Button("⏴", bcb) | vcenter;
  auto right_btn = Button("⏵", bcb) | vcenter;
  auto down_btn = Button("⏷", bcb) | hcenter;

  auto main_renderer = Renderer(grid_component, [&] {
    return vbox({
               hbox({up_btn->Render()}) | hcenter,
               separator(),
               hbox({left_btn->Render(), separator(), grid_element, separator(),
                     right_btn->Render()}) |
                   hcenter,
               separator(),
               hbox({down_btn->Render()}) | hcenter,
           }) |
           center;
  });

  screen.Loop(main_renderer);
}
