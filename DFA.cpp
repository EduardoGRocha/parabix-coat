#include "perfevent/PerfEvent.hpp"

#include "utils.h"


int main(int argc, char* argv[]) {
  uint8_t state0[256];
  uint8_t state1[256];
  uint8_t state2[256];

  std::vector<uint8_t *> DFA = { state0, state1, state2 };

  // Build automata for 130-[0:9]*-140
  for (size_t i = 0; i < 256; ++i) {
    state0[i] = 0;
    state1[i] = 0;
    state2[i] = 0;

    if ( i == 130 ) {
      state0[i] = 1;
    } else if ( i <= 9 ) {
      state1[i] = 1;
    } else if ( i == 140 ) {
      state1[i] = 2;
    }
  }

  size_t N_ROWS = 256*1024*1024;
  std::vector<uint8_t> input = sequential_data(N_ROWS);
  auto output = std::vector<uint8_t>(N_ROWS, 0);

  std::vector<size_t> workload_sizes = {
      1024, 4*1024, 16*1024, 64*1024, 256*1024, 1024*1024,
      4*1024*1024, 16*1024*1024, 64*1024*1024, 256*1024*1024
  };

  for (const auto& workload_size : workload_sizes) {
    size_t n_iterations = 20;
    PerfEvent e;
    e.startCounters();
    for (size_t iteration = 0; iteration < n_iterations; ++iteration) {
      size_t cur_state = 0;
      for (size_t i = 0; i < workload_size; ++i) {
        const auto& in_ = input[i];
        cur_state = (DFA[cur_state])[in_];
        output[i] = (uint8_t) (cur_state == 2);
      }
    }
    e.stopCounters();
    // Normalize per Byte
    e.printReport(std::cout, n_iterations*workload_size); // use n as scale factor
    std::cout << std::endl;
  }

  return 0;
}
