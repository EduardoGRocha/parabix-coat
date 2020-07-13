#include <iostream>
#include <emmintrin.h>
#include <vector>
#include <bitset>

#include "scalar/operations_64.h"
#include "scalar/character_database_64.h"

#define N_ROWS 64 * 2

void sse_trans(uint8_t const *inp, uint8_t *out, int nrows, int ncols) {
#   define INP(x,y) inp[(x)*ncols/8 + (y)/8]
#   define OUT(x,y) out[(y)*nrows/8 + (x)/8]
  int rr, cc, i, h;
  union { __m128i x; uint8_t b[16]; } tmp{};

  // Do the main body in 16x8 blocks:
  for (rr = 0; rr <= nrows - 16; rr += 16) {
    for (cc = 0; cc < ncols; cc += 8) {
      for (i = 0; i < 16; ++i)
        tmp.b[i] = INP(rr + i, cc);
//      for (i = 0; i < 8; ++i)
//        tmp.b[7 - i] = INP(rr + i, cc);
//      for (i = 0; i < 8; ++i)
//        tmp.b[15 - i] = INP(rr + 8 + i, cc);
      for (i = 0; i < 8; ++i, tmp.x = _mm_slli_epi64(tmp.x, 1))
        *(uint16_t*)&OUT(rr,cc+i)= _mm_movemask_epi8(tmp.x);
    }
  }
  if (rr == nrows) return;

  // The remainder is a block of 8x(16n+8) bits (n may be 0).
  //  Do a PAIR of 8x8 blocks in each step:
  for (cc = 0; cc <= ncols - 16; cc += 16) {
    for (i = 0; i < 8; ++i) {
      tmp.b[i] = h = *(uint16_t const*)&INP(rr + i, cc);
      tmp.b[i + 8] = h >> 8;
    }
    for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1)) {
      OUT(rr, cc + i) = h = _mm_movemask_epi8(tmp.x);
      OUT(rr, cc + i + 8) = h >> 8;
    }
  }
  if (cc == ncols) return;

  //  Do the remaining 8x8 block:
  for (i = 0; i < 8; ++i)
    tmp.b[i] = INP(rr + i, cc);
  for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1))
    OUT(rr, cc + i) = _mm_movemask_epi8(tmp.x);
}

int main() {
  std::vector<uint8_t> input{};

  for (size_t i = 0; i < N_ROWS; ++i) {
    input.push_back(i);
  }

  auto output = std::vector<uint8_t>(N_ROWS, 0);

  sse_trans(input.data(), output.data(), N_ROWS, 8);


  for (const auto& i : input) {
    std::cout << std::bitset<8>(i) << "\n";
  }

  std::cout << "---------------- \n";

  for (size_t i = 0; i < output.size(); ) {
    for (size_t j = 0; j < N_ROWS/8; ++j) {
      std::cout << std::bitset<8>(output[i]) << "";
      ++i;
    }
    std::cout << "\n";
  }

  auto stream_7 = reinterpret_cast<uint64_t *>(output.data());
  auto stream_6 = reinterpret_cast<uint64_t *>(output.data()) + 2;
  auto stream_5 = reinterpret_cast<uint64_t *>(output.data()) + 4;
  auto stream_4 = reinterpret_cast<uint64_t *>(output.data()) + 6;
  auto stream_3 = reinterpret_cast<uint64_t *>(output.data()) + 8;
  auto stream_2 = reinterpret_cast<uint64_t *>(output.data()) + 10;
  auto stream_1 = reinterpret_cast<uint64_t *>(output.data()) + 12;
  auto stream_0 = reinterpret_cast<uint64_t *>(output.data()) + 14;

  for (size_t i = 0; i < 2; ++i) {
    auto matched_0_9 = match_0_9(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
        );
    auto matched_0 = match_0(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
        );
    auto matched_1 = match_1(
        *(stream_7 + i), *(stream_6 + i), *(stream_5 + i), *(stream_4 + i),
        *(stream_3 + i), *(stream_2 + i), *(stream_1 + i), *(stream_0 + i)
      );

    auto scanned_through = scan_through(matched_0 << 1, matched_1);
    auto matched_star = match_star(matched_0 << 1, matched_0_9);

    std::cout << std::bitset<64>(*(stream_7 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_6 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_5 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_4 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_3 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_2 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_1 + i)) << "\n";
    std::cout << std::bitset<64>(*(stream_0 + i)) << "\n";

    std::cout << std::bitset<64>(matched_1) << "\n";
    std::cout << std::bitset<64>(matched_0 << 1) << "\n";
    std::cout << std::bitset<64>(scanned_through) << "\n";
    std::cout << std::bitset<64>(matched_star) << "\n";
    std::cout << "\n";
  }

  return 0;
}
