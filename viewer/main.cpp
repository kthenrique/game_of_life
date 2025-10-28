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
  auto title = vbox({
                   text("┏━╸┏━┓┏┳┓┏━╸ ┏━┓┏━╸ ╻  ╻┏━╸┏━╸"),
                   text("┃╺┓┣━┫┃┃┃┣╸  ┃ ┃┣╸  ┃  ┃┣╸ ┣╸ "),
                   text("┗━┛╹ ╹╹ ╹┗━╸ ┗━┛╹   ┗━╸╹╹  ┗━╸"),

               }) |
               bold | hcenter;

  auto author_line = text("by Kelve T. Henrique") | dim | align_right;

  auto header = vbox({
      title,
      author_line,
  });

  auto alive = "■";
  auto dead = "□";

  auto screen = ::ftxui::ScreenInteractive::Fullscreen();
  Cell window_size{26, 26};

  auto epoch_period = std::chrono::milliseconds(1500);
  auto const epoch_period_increment = std::chrono::milliseconds(100);
  std::size_t epoch{0};

  Cells initial_seed = std::unordered_set<Cell>{
      {-8, 2},  {-8, 1}, {-8, 0},  {-8, -1}, {-8, -2}, {-7, 0}, {-6, 1},
      {-6, -1}, {-5, 2}, {-5, -2}, {-3, -2},

      {0, 2},   {0, 1},  {0, 0},   {0, -1},  {0, -2},  {-2, 2}, {-1, 2},
      {0, 2},   {1, 2},  {2, 2},   {2, -2},

      {5, 2},   {5, 1},  {5, 0},   {5, -1},  {5, -2},  {6, 0},  {7, 0},
      {8, 2},   {8, 1},  {8, 0},   {8, -1},  {8, -2},  {10, -2}};
  auto gen = Generation<Cells>(initial_seed);

  std::atomic<int64_t> offset_x{0};
  std::atomic<int64_t> offset_y{0};

  int runtime_ctrl_selected = 2; // begin paused
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
      Cell const transformed = {.x = cell.x + 10 + offset_x,
                                .y = std::abs(cell.y - 10) + offset_y};
      if (transformed.x < 0 || transformed.y < 0 ||
          transformed.x >= window_size.x || transformed.y >= window_size.y) {
        continue;
      }

      planet[transformed.y][transformed.x] =
          text(alive) | center | size(WIDTH, EQUAL, 2) | size(HEIGHT, EQUAL, 1);
    }
  };

  // Refresher
  std::thread([&] {
    {
      std::lock_guard<std::mutex> lg(planet_mutex);

      update_window_view();
    }
    screen.PostEvent(Event::Custom);
    while (true) {
      switch (runtime_ctrl_selected) {
      case 3: { // stop
        screen.Post([&] { screen.Exit(); });
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

  std::string tooltip;
  auto erase_tooltip = [&] { tooltip = ""; };

  auto up_btn = Hoverable(
      Button("⏶", update_offset(1, offset_y)),
      [&] { tooltip = "Move view up"; }, erase_tooltip);
  auto left_btn = Hoverable(
      Button("⏴", update_offset(1, offset_x)),
      [&] { tooltip = "Move view left"; }, erase_tooltip);
  auto right_btn = Hoverable(
      Button("⏵", update_offset(-1, offset_x)),
      [&] { tooltip = "Move view right"; }, erase_tooltip);
  auto down_btn = Hoverable(
      Button("⏷", update_offset(-1, offset_y)),
      [&] { tooltip = "Move view down"; }, erase_tooltip);

  auto slow_time_btn = Hoverable(
      Button("⏮", [&] { runtime_ctrl_selected = 0; }),
      [&] { tooltip = "🛈 Increase epoch period by 100ms"; }, erase_tooltip);
  auto play_btn = Hoverable(
      Button("⏵", [&] { runtime_ctrl_selected = 1; }),
      [&] { tooltip = "🛈 Continue"; }, erase_tooltip);
  auto pause_time_btn = Hoverable(
      Button("⏸", [&] { runtime_ctrl_selected = 2; }),
      [&] { tooltip = "🛈 Pause time"; }, erase_tooltip);
  auto stop_btn = Hoverable(
      Button("⏹", [&] { runtime_ctrl_selected = 3; }),
      [&] { tooltip = "🛈 Exit game"; }, erase_tooltip);
  auto speed_time_btn = Hoverable(
      Button("⏭", [&] { runtime_ctrl_selected = 4; }),
      [&] { tooltip = "🛈 Decrease epoch period by 100ms"; }, erase_tooltip);

  auto controls = Container::Horizontal(
      {up_btn, left_btn, right_btn, down_btn, slow_time_btn, play_btn,
       pause_time_btn, stop_btn, speed_time_btn});

  auto main_renderer = Renderer(controls, [&] {
    std::lock_guard<std::mutex> lg(planet_mutex);
    auto epoch_label =
        text(std::format("Epoch #{:05} {}", epoch, clock_frames[frame]));
    auto label =
        text(std::format("🖸({}, {})", -1 * offset_x.load(), offset_y.load()));
    auto tooltip_label = text(std::format("{}", tooltip));

    return vbox({header | hcenter, hbox({epoch_label | flex, label}),
                 separator(),
                 hbox({gridbox(planet) | border | center}) | hcenter,
                 hbox({hbox({
                           up_btn->Render(),
                           left_btn->Render(),
                           right_btn->Render(),
                           down_btn->Render(),
                       }) | border,
                       filler(),
                       hbox({slow_time_btn->Render(), play_btn->Render(),
                             pause_time_btn->Render(), stop_btn->Render(),
                             speed_time_btn->Render()}) |
                           border}),
                 tooltip == "" ? vbox({text(""), text("")})
                               : vbox({separator(), tooltip_label})}) |
           center;
  });

  screen.Loop(main_renderer);
}
