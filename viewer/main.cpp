#include <cells.hpp>
#include <chrono>
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <generation.hpp>
#include <ranges>

using namespace ftxui;
using ::ftxui::center;
using ::ftxui::Element;
using ::ftxui::Elements;
using ::ftxui::gridbox;
using ::ftxui::separator;
using ::ftxui::size;
using ::ftxui::text;

int main() {
  auto alive = "■";
  auto dead = "□";

  auto screen = ::ftxui::ScreenInteractive::Fullscreen();
  Cell window_size{21, 21};

  auto epoch_period = std::chrono::milliseconds(1500);
  auto epoch_period_increment = std::chrono::milliseconds(100);
  std::size_t epoch{0};

  Cells initial_seed = std::unordered_set<Cell>{{0, 0}, {1, 0}, {2, 0}, {3, 0}};
  auto gen = Generation<Cells>(initial_seed);

  std::atomic<int64_t> offset_x{0};
  std::atomic<int64_t> offset_y{0};

  int runtime_ctrl_selected = 1;
  std::vector<std::string> runtime_entries = {"⏮", "⏵", "⏸", "⏹", "⏭"};
  auto reaction_to_runtime_ctrl = std::chrono::milliseconds(500);

  std::array<std::string, 2> clock_frames = {"●", "○"};
  std::atomic<int> frame{0};

  std::mutex planet_mutex;
  std::vector<Elements> planet;

  // Initialize planet
  planet.reserve(window_size.y);
  for (int y = 0; y < window_size.y; y++) {
    std::vector<Element> longitude;
    longitude.reserve(window_size.x);
    for (int x = 0; x < window_size.x; x++) {
      Element node =
          text(dead) | center | size(WIDTH, EQUAL, 2) | size(HEIGHT, EQUAL, 1);
      longitude.push_back(node);
    }
    planet.push_back(longitude);
  }

  auto update_window_view = [&] {
    for (int y = 0; y < window_size.y; y++) {
      for (int x = 0; x < window_size.x; x++) {
        planet[x][y] = text(dead) | center | size(WIDTH, EQUAL, 2) |
                       size(HEIGHT, EQUAL, 1);
      }
    }

    auto const &cells = gen.get_alive_cells();
    for (auto const &cell : cells) {
      Cell const transformed = {.x = std::abs(cell.x - 10) + offset_y,
                                .y = cell.y + 10 + offset_x};
      if (transformed.x < 0 || transformed.y < 0 ||
          transformed.x >= window_size.x || transformed.y >= window_size.y) {
        continue;
      }

      planet[transformed.x][transformed.y] =
          text(alive) | center | size(WIDTH, EQUAL, 2) | size(HEIGHT, EQUAL, 1);
    }
  };

  // Refresher
  std::thread([&] {
    while (true) {
      switch (runtime_ctrl_selected) {
      case 3: { // stop
        screen.PostEvent(Event::End);
        return;
      }
      case 2: { // pause
        std::this_thread::sleep_for(reaction_to_runtime_ctrl);
        continue;
      }
      case 4: { // speed epoch period up
        epoch_period -= epoch_period_increment;
        runtime_ctrl_selected = 1;
        continue;
      }
      case 0: { // slow epoch period down
        epoch_period += epoch_period_increment;
        runtime_ctrl_selected = 1;
        continue;
      }
      case 1: // play
      default: {
        // nothing todo
      }
      }
      frame = (frame + 1) % clock_frames.size();
      {
        std::lock_guard<std::mutex> lg(planet_mutex);

        update_window_view();
      }
      screen.PostEvent(Event::Custom);
      std::this_thread::sleep_for(epoch_period);
      gen.next();
      ++epoch;
    }
  }).detach();

  auto update_offset = [&](int64_t update, std::atomic<int64_t> &offset) {
    return [&, update]() {
      std::lock_guard<std::mutex> lg(planet_mutex);
      offset += update;
      update_window_view();
      screen.PostEvent(Event::Custom);
    };
  };

  auto up_btn = Button("⏶", update_offset(1, offset_y));
  auto left_btn = Button("⏴", update_offset(1, offset_x));
  auto right_btn = Button("⏵", update_offset(-1, offset_x));
  auto down_btn = Button("⏷", update_offset(-1, offset_y));

  std::vector<Component> runtime_ctrl_btn;
  for (int i = 0; i < runtime_entries.size(); ++i) {
    runtime_ctrl_btn.push_back(
        Button(runtime_entries[i], [&, i] { runtime_ctrl_selected = i; }));
  }
  auto runtime_ctrl_container = Container::Horizontal(runtime_ctrl_btn);

  auto controls = Container::Horizontal(
      {up_btn, left_btn, right_btn, down_btn, runtime_ctrl_container});
  auto main_renderer = Renderer(controls, [&] {
    std::lock_guard<std::mutex> lg(planet_mutex);
    auto epoch_label =
        text(std::format("Epoch #{:5} {}", epoch, clock_frames[frame]));
    auto label =
        text(std::format("🖸({}, {})", -1 * offset_x.load(), offset_y.load()));
    return vbox({hbox({epoch_label | flex, label}), separator(),
                 hbox({gridbox(planet) | border | center}) | hcenter,
                 hbox({hbox({
                           up_btn->Render(),
                           left_btn->Render(),
                           right_btn->Render(),
                           down_btn->Render(),
                       }) | border,
                       hbox({runtime_ctrl_container->Render()}) | border |
                           flex})}) |
           center;
  });

  screen.Loop(main_renderer);
}
