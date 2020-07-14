#include <iostream>
#include <emmintrin.h>
#include <vector>
#include <bitset>

#include "utils.h"
#include "scalar/operations_64.h"
#include "scalar/character_database_64.h"

#define N_VECTORS 2
#define N_ROWS 64 * N_VECTORS


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


int main() {
//  auto input = random_byte_data(N_ROWS);
//  auto input = sequential_data(N_ROWS);
  std::vector<uint8_t> input {
      130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140,
      130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 3,
      9, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140,
      130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 11, 1, 1, 140, 130, 0, 0, 0, 10, 1, 1, 140
  };

  auto output = std::vector<uint8_t>(N_ROWS, 0);
  auto output_stream = std::vector<uint64_t>(N_ROWS, 0);

  transpose_sse(input.data(), output.data(), N_ROWS, 8);

  regex_match_130_09_star_140(reinterpret_cast<uint64_t *>(output.data()), output_stream.data(), 2);

  auto stream_7 = reinterpret_cast<uint64_t *>(output.data());
  auto stream_6 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS;
  auto stream_5 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 2;
  auto stream_4 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 3;
  auto stream_3 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 4;
  auto stream_2 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 5;
  auto stream_1 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 6;
  auto stream_0 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 7;
//
//  uint64_t matched_0[N_VECTORS];
//  uint64_t matched_1[N_VECTORS];
//  uint64_t matched_0_9[N_VECTORS];
//  uint64_t scanned_through[N_VECTORS];
//  uint64_t matched_star[N_VECTORS];
//
//  for (size_t i = 0; i < N_VECTORS; ++i) {
//    matched_0_9[i] = match_0_9(
//        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
//        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
//        );
//    matched_0[i] = match_0(
//        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
//        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
//        );
//    matched_1[i] = match_1(
//        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
//        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
//      );
//
//    scanned_through[i] = scan_thru(matched_0[i] << 1, matched_1[i]);
//    matched_star[i] = match_star(matched_0[i] << 1, matched_0_9[i]);
//  }
//
  print_bit_array(stream_7, N_VECTORS);
  print_bit_array(stream_6, N_VECTORS);
  print_bit_array(stream_5, N_VECTORS);
  print_bit_array(stream_4, N_VECTORS);
  print_bit_array(stream_3, N_VECTORS);
  print_bit_array(stream_2, N_VECTORS);
  print_bit_array(stream_1, N_VECTORS);
  print_bit_array(stream_0, N_VECTORS);

  line_break();

  print_bit_array(output_stream.data(), N_VECTORS);

//  print_bit_array(matched_0, N_VECTORS);
//  print_bit_array(matched_1, N_VECTORS);
//  print_bit_array(matched_0_9, N_VECTORS);
//  print_bit_array(scanned_through, N_VECTORS);
//  print_bit_array(matched_star, N_VECTORS);

  return 0;
}
