#include <cells.hpp>
#include <chrono>
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <generation.hpp>
#include <ranges>
#include <theme.hpp>
#include <utils.hpp>

using ::ftxui::bold;
using ::ftxui::border;
using ::ftxui::borderRounded;
using ::ftxui::Button;
using ::ftxui::center;
using ::ftxui::Element;
using ::ftxui::Elements;
using ::ftxui::filler;
using ::ftxui::flex;
using ::ftxui::gridbox;
using ::ftxui::hbox;
using ::ftxui::hcenter;
using ::ftxui::separator;
using ::ftxui::text;
using ::ftxui::vbox;
using ::ftxui::Container::Horizontal;

int main() {
  auto header = utils::get_header();
  auto screen = ::ftxui::ScreenInteractive::Fullscreen();
  Cell window_size{36, 36};

  auto epoch_period = std::chrono::milliseconds(1000);
  auto const epoch_period_increment = std::chrono::milliseconds(100);
  std::size_t epoch{0};

  Cells initial_seed = std::unordered_set<Cell>{
      {-6, 2},  {-6, 1}, {-6, 0},  {-6, -1}, {-6, -2}, {-5, 0}, {-4, 1},
      {-4, -1}, {-3, 2}, {-3, -2}, {-1, -2},

      {2, 2},   {2, 1},  {2, 0},   {2, -1},  {2, -2},  {0, 2},  {1, 2},
      {2, 2},   {3, 2},  {4, 2},   {4, -2},

      {7, 2},   {7, 1},  {7, 0},   {7, -1},  {7, -2},  {8, 0},  {9, 0},
      {10, 2},  {10, 1}, {10, 0},  {10, -1}, {10, -2}, {12, -2}};
  auto gen = Generation<Cells>(initial_seed);

  std::atomic<int64_t> offset_x{0};
  std::atomic<int64_t> offset_y{0};

  utils::Runtime_ctrl runtime_ctrl_selected = utils::Runtime_ctrl::pause;
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
      Element node = utils::dead_cell();
      longitude.push_back(node);
    }
    planet.push_back(longitude);
  }

  auto update_window_view = [&] {
    for (int y = 0; y < window_size.y; y++) {
      for (int x = 0; x < window_size.x; x++) {
        planet[x][y] = utils::dead_cell();
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

      planet[transformed.y][transformed.x] = utils::alive_cell();
    }
  };

  // Refresher
  std::thread([&] {
    {
      std::lock_guard<std::mutex> lg(planet_mutex);

      update_window_view();
    }
    screen.PostEvent(ftxui::Event::Custom);
    while (true) {
      switch (runtime_ctrl_selected) {
      case utils::Runtime_ctrl::stop: {
        epoch = 0;
        gen = Generation<Cells>(initial_seed);
        runtime_ctrl_selected = utils::Runtime_ctrl::pause;
        {
          std::lock_guard<std::mutex> lg(planet_mutex);

          update_window_view();
        }
        screen.PostEvent(ftxui::Event::Custom);
        continue;
      }
      case utils::Runtime_ctrl::pause: {
        std::this_thread::sleep_for(reaction_to_runtime_ctrl);
        continue;
      }
      case utils::Runtime_ctrl::fast_forward: { // speed epoch period up
        if (epoch_period < epoch_period_increment) {
          epoch_period = std::chrono::milliseconds(0);
        } else {
          epoch_period -= epoch_period_increment;
        }
        runtime_ctrl_selected = utils::Runtime_ctrl::start;
        screen.PostEvent(ftxui::Event::Custom);
        continue;
      }
      case utils::Runtime_ctrl::rewind: { // slow epoch period down
        epoch_period += epoch_period_increment;
        runtime_ctrl_selected = utils::Runtime_ctrl::start;
        screen.PostEvent(ftxui::Event::Custom);
        continue;
      }
      case utils::Runtime_ctrl::start:
        [[fallthrough]];
      default: {
        // nothing todo
      }
      }
      frame = (frame + 1) % clock_frames.size();
      {
        std::lock_guard<std::mutex> lg(planet_mutex);

        update_window_view();
      }
      screen.PostEvent(ftxui::Event::Custom);
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
      screen.PostEvent(ftxui::Event::Custom);
    };
  };

  std::string tooltip;
  auto erase_tooltip = [&] { tooltip = ""; };
  auto update_tooltip = [&](std::string tt) {
    return [&tooltip, tt_ = std::move(tt)] { tooltip = tt_; };
  };

  auto up_btn = Hoverable(Button(" ⏶ ", update_offset(1, offset_y)),
                          update_tooltip("🛈 Move view up"), erase_tooltip);
  auto left_btn = Hoverable(Button(" ⏴ ", update_offset(1, offset_x)),
                            update_tooltip("🛈 Move view left"), erase_tooltip);
  auto right_btn =
      Hoverable(Button(" ⏵ ", update_offset(-1, offset_x)),
                update_tooltip("🛈 Move view right"), erase_tooltip);
  auto down_btn = Hoverable(Button(" ⏷ ", update_offset(-1, offset_y)),
                            update_tooltip("🛈 Move view down"), erase_tooltip);

  auto exit_btn = Hoverable(
      Button(" ⏻ ", [&] { screen.Exit(); }), [&] { tooltip = "🛈 Exit"; },
      erase_tooltip);

  auto slow_time_btn = Hoverable(
      Button(" ⏮ ",
             [&] { runtime_ctrl_selected = utils::Runtime_ctrl::rewind; }),
      [&] {
        tooltip = std::format("🛈 Increase epoch period by {} - currently at {}",
                              epoch_period_increment, epoch_period);
      },
      erase_tooltip);
  auto play_btn = Hoverable(
      Button(" ⏵ ",
             [&] { runtime_ctrl_selected = utils::Runtime_ctrl::start; }),
      update_tooltip("🛈 Continue"), erase_tooltip);
  auto pause_time_btn = Hoverable(
      Button(" ⏸ ",
             [&] { runtime_ctrl_selected = utils::Runtime_ctrl::pause; }),
      update_tooltip("🛈 Pause time"), erase_tooltip);
  auto stop_btn = Hoverable(
      Button(" ⏹ ", [&] { runtime_ctrl_selected = utils::Runtime_ctrl::stop; }),
      update_tooltip("🛈 Exit game"), erase_tooltip);
  auto speed_time_btn = Hoverable(
      Button(
          " ⏭ ",
          [&] { runtime_ctrl_selected = utils::Runtime_ctrl::fast_forward; }),
      [&] {
        tooltip = std::format("🛈 Decrease epoch period by {} - currently at {}",
                              epoch_period_increment, epoch_period);
      },
      erase_tooltip);

  auto controls = Horizontal({up_btn, left_btn, right_btn, down_btn, exit_btn,
                              slow_time_btn, play_btn, pause_time_btn, stop_btn,
                              speed_time_btn});

  auto main_renderer = Renderer(controls, [&] {
    std::lock_guard<std::mutex> lg(planet_mutex);

    auto epoch_label =
        text(std::format("Epoch #{:05} {}", epoch, clock_frames[frame]));
    auto label =
        text(std::format("🖸 ({}, {})", -1 * offset_x.load(), offset_y.load()));
    auto tooltip_label = text(std::format("{}", tooltip));

    auto status = hbox({epoch_label | flex, label});
    auto grid =
        hbox({gridbox(planet) | theme::grid_bg_color | border | center});

    auto viewport_buttons = hbox({
        up_btn->Render(),
        left_btn->Render(),
        right_btn->Render(),
        down_btn->Render(),
    });
    auto simulation_buttons = hbox(
        {slow_time_btn->Render(), play_btn->Render(), pause_time_btn->Render(),
         stop_btn->Render(), speed_time_btn->Render()});

    auto tooltip_txt = tooltip == "" ? vbox({text(""), text("")})
                                     : vbox({separator(), tooltip_label});
    return vbox({header | hcenter, status, separator(), grid | hcenter,
                 hbox({viewport_buttons | bold | borderRounded, filler(),
                       exit_btn->Render() | theme::main_color | borderRounded,
                       filler(), simulation_buttons | bold | borderRounded}) |
                     bold | borderRounded,
                 tooltip_txt}) |
           center;
  });

  screen.Loop(main_renderer);
}
