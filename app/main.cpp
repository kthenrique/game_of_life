#include <cells.hpp>
#include <chrono>
#include <generation.hpp>
#include <iostream>
#include <ranges>
#include <thread>

int main(int argc, char *argv[]) {
  auto nr_epochs = 5;
  auto epoch_period = std::chrono::seconds(1);
  std::size_t epoch{0};
  Cells initial_seed = std::unordered_set<Cell>{{0, 0}, {1, 0}, {2, 0}, {3, 0}};
  auto gen = Generation<Cells>(initial_seed);

  std::cout << "Initial generation:" << std::endl;
  std::cout << gen << std::endl;

  std::cout << "\nStart of life:" << std::endl;
  for ([[maybe_unused]] auto _ : std::ranges::views::iota(0, nr_epochs)) {
    std::cout << "EPOCH nr. " << epoch++ << ": " << gen << std::endl;
    gen.next();
    std::this_thread::sleep_for(epoch_period);
  }
  std::cout << "End of life\n" << std::endl;

  std::cout << "Final generation:" << std::endl;
  std::cout << gen << std::endl;
  return 0;
}
