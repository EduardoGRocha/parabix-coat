#include <iostream>
#include <emmintrin.h>
#include <vector>
#include <bitset>

#include "utils.h"
#include "scalar/operations_64.h"
#include "scalar/character_database_64.h"

#define N_VECTORS 4
#define N_ROWS 64 * N_VECTORS


int main() {
//  auto input = random_byte_data(N_ROWS);
  auto input = sequential_data(N_ROWS);

  auto output = std::vector<uint8_t>(N_ROWS, 0);

  transpose_sse(input.data(), output.data(), N_ROWS, 8);

  auto stream_7 = reinterpret_cast<uint64_t *>(output.data());
  auto stream_6 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS;
  auto stream_5 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 2;
  auto stream_4 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 3;
  auto stream_3 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 4;
  auto stream_2 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 5;
  auto stream_1 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 6;
  auto stream_0 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 7;

  uint64_t matched_0[N_VECTORS];
  uint64_t matched_1[N_VECTORS];
  uint64_t matched_0_9[N_VECTORS];
  uint64_t scanned_through[N_VECTORS];
  uint64_t matched_star[N_VECTORS];

  for (size_t i = 0; i < N_VECTORS; ++i) {
    matched_0_9[i] = match_0_9(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
        );
    matched_0[i] = match_0(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
        );
    matched_1[i] = match_1(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
      );

    scanned_through[i] = scan_through(matched_0[i] << 1, matched_1[i]);
    matched_star[i] = match_star(matched_0[i] << 1, matched_0_9[i]);
  }

  print_bit_array(stream_7, N_VECTORS);
  print_bit_array(stream_6, N_VECTORS);
  print_bit_array(stream_5, N_VECTORS);
  print_bit_array(stream_4, N_VECTORS);
  print_bit_array(stream_3, N_VECTORS);
  print_bit_array(stream_2, N_VECTORS);
  print_bit_array(stream_1, N_VECTORS);
  print_bit_array(stream_0, N_VECTORS);

  line_break();

  print_bit_array(matched_0, N_VECTORS);
  print_bit_array(matched_1, N_VECTORS);
  print_bit_array(matched_0_9, N_VECTORS);
  print_bit_array(scanned_through, N_VECTORS);
  print_bit_array(matched_star, N_VECTORS);

  return 0;
}
