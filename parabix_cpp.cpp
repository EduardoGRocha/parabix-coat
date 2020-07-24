#include <vector>
#include <bitset>
#include <chrono>

#include "utils.h"
#include "scalar/operations_64.h"
#include "scalar/character_database_64.h"

#include "perfevent/PerfEvent.hpp"


void regex_match_130_09_star_140(uint64_t* input, uint64_t* output, size_t size) {
  // Matches (130, [0-9]*, 140) ReGex
  auto stream_B7 = input;
  auto stream_B6 = input + size;
  auto stream_B5 = input + size * 2;
  auto stream_B4 = input + size * 3;
  auto stream_B3 = input + size * 4;
  auto stream_B2 = input + size * 5;
  auto stream_B1 = input + size * 6;
  auto stream_B0 = input + size * 7;

  uint64_t carry_advance_130 = 0;
  uint64_t carry_match_star = 0;
  uint64_t carry_advance_140 = 0;

  for (size_t i = 0; i < size; ++i) {
    auto B7 = *(stream_B7 + i);
    auto B6 = *(stream_B6 + i);
    auto B5 = *(stream_B5 + i);
    auto B4 = *(stream_B4 + i);
    auto B3 = *(stream_B3 + i);
    auto B2 = *(stream_B2 + i);
    auto B1 = *(stream_B1 + i);
    auto B0 = *(stream_B0 + i);

    auto matched_130 = match_130(B7, B6, B5, B4, B3, B2, B1, B0);
    auto matched_0_9 = match_0_9(B7, B6, B5, B4, B3, B2, B1, B0);
    auto matched_140 = match_140(B7, B6, B5, B4, B3, B2, B1, B0);

    auto r_advanced_130 = advance(matched_130, carry_advance_130);
    auto advanced_130 = r_advanced_130.result;
    carry_advance_130 = r_advanced_130.carry;

    auto r_match_star = match_star(advanced_130, matched_0_9, carry_match_star);
    auto matched_star = r_match_star.result;
    carry_match_star = r_match_star.carry;

    auto r_advanced_140 = advance(matched_star & matched_140, carry_advance_140);
    output[i] = r_advanced_140.result;
    carry_advance_140 = r_advanced_140.carry;
  }
}


//void test_regex() {
//  auto stream_7 = reinterpret_cast<uint64_t *>(output.data());
//  auto stream_6 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS;
//  auto stream_5 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 2;
//  auto stream_4 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 3;
//  auto stream_3 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 4;
//  auto stream_2 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 5;
//  auto stream_1 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 6;
//  auto stream_0 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 7;
//
//  print_bit_array(stream_7, N_VECTORS);
//  print_bit_array(stream_6, N_VECTORS);
//  print_bit_array(stream_5, N_VECTORS);
//  print_bit_array(stream_4, N_VECTORS);
//  print_bit_array(stream_3, N_VECTORS);
//  print_bit_array(stream_2, N_VECTORS);
//  print_bit_array(stream_1, N_VECTORS);
//  print_bit_array(stream_0, N_VECTORS);
//
//  line_break();
//
//  print_bit_array(output_stream.data(), N_VECTORS);
//}


void benchmark_transpose() {
  __asm volatile("");
  std::vector<uint8_t> input = sequential_data(256*1024*1024);
  auto N_ROWS = input.size();
  auto output = std::vector<uint8_t>(N_ROWS, 0);

  size_t n = 50;
  PerfEvent e;
  e.startCounters();
  for (size_t i = 0; i < n; ++i) {
    transpose_sse(input.data(), output.data(), N_ROWS, 8);
  }
  e.stopCounters();
  e.printReport(std::cout, n); // use n as scale factor
  std::cout << std::endl;
}

void benchmark_cpp(uint64_t *input, uint64_t *output_stream) {
  std::vector<size_t> workload_sizes = {
      1024, 4*1024, 16*1024, 64*1024, 256*1024, 1024*1024,
      4*1024*1024, 16*1024*1024, 64*1024*1024, 256*1024*1024
  };

  for (const auto& workload_size : workload_sizes) {
    size_t N_VECTORS = workload_size/64;
    size_t n_iterations = 20;
    PerfEvent e;
    e.startCounters();
    for (size_t iteration = 0; iteration < n_iterations; ++iteration) {
      regex_match_130_09_star_140(input, output_stream, N_VECTORS);
    }
    e.stopCounters();
    // Normalize per Byte
    e.printReport(std::cout, n_iterations*workload_size); // use n as scale factor
    std::cout << std::endl;
  }
}


int main() {
#if true
/// Benchmark execution
//  std::vector<uint8_t> input = small_test_sequence();
//  std::vector<uint8_t> input = random_byte_data(512*1024*1024);
// TODO throwing free(): invalid pointer for size > 256*1024*1024
// The problems seems to be on transpose_sse
  size_t N_ROWS = 256*1024*1024;
  std::vector<uint8_t> input = sequential_data(N_ROWS);

  auto transposed_input = std::vector<uint8_t>(N_ROWS, 0);
  auto output_stream = std::vector<uint64_t>(N_ROWS, 0);

  transpose_sse(input.data(), transposed_input.data(), N_ROWS, 8);
  uint64_t *transposed_input_64 = reinterpret_cast<uint64_t*>(transposed_input.data());

  benchmark_cpp(transposed_input_64, output_stream.data());
#else
/// Benchmark transposing
  benchmark_transpose();
#endif
  return 0;
}
