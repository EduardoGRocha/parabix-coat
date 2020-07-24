#pragma once

#include <iostream>
#include <emmintrin.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>

#define VECTOR_LENGTH 64

void transpose_sse(uint8_t const *inp, uint8_t *out, int n_rows, int n_cols) {
#   define INP(x,y) inp[(x)*n_cols/8 + (y)/8]
#   define OUT(x,y) out[(y)*n_rows/8 + (x)/8]
  int rr, cc, i, h;
  union { __m128i x; uint8_t b[16]; } tmp{};

  // Do the main body in 16x8 blocks:
  for (rr = 0; rr <= n_rows - 16; rr += 16) {
    for (cc = 0; cc < n_cols; cc += 8) {
      for (i = 0; i < 16; ++i)
        tmp.b[i] = INP(rr + i, cc);
      for (i = 0; i < 8; ++i, tmp.x = _mm_slli_epi64(tmp.x, 1))
        *(uint16_t*)&OUT(rr,cc+i)= _mm_movemask_epi8(tmp.x);
    }
  }
  if (rr == n_rows) return;

//  std::cout << "------------";
  // The remainder is a block of 8x(16n+8) bits (n may be 0).
  //  Do a PAIR of 8x8 blocks in each step:
  for (cc = 0; cc <= n_cols - 16; cc += 16) {
    for (i = 0; i < 8; ++i) {
      tmp.b[i] = h = *(uint16_t const*)&INP(rr + i, cc);
      tmp.b[i + 8] = h >> 8;
    }
    for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1)) {
      OUT(rr, cc + i) = h = _mm_movemask_epi8(tmp.x);
      OUT(rr, cc + i + 8) = h >> 8;
    }
  }
  if (cc == n_cols) return;

  //  Do the remaining 8x8 block:
  for (i = 0; i < 8; ++i)
    tmp.b[i] = INP(rr + i, cc);
  for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1))
    OUT(rr, cc + i) = _mm_movemask_epi8(tmp.x);
}

void print_bit_array(uint64_t* array, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    std::cout << std::bitset<64>(*(array + i)) << " ";
  }
  std::cout << "\n";
}

void line_break() {
  std::cout << "------------------------ \n";
}

std::vector<uint8_t> small_test_sequence() {
  return {
    130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140,
    130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 3,
    9, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140,
    130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 1, 1, 1, 140, 130, 0, 0, 0, 11, 1, 1, 140, 130, 0, 0, 0, 10, 1, 1, 140
  };
}

std::vector<uint8_t> random_byte_data(size_t size) {
  std::vector<uint8_t> ret(size);

  std::random_device rnd_device;
  std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
  std::uniform_int_distribution<uint8_t> dist{1, std::numeric_limits<uint8_t>::max()-1};

  auto gen = [&dist, &mersenne_engine](){
    return dist(mersenne_engine);
  };

  std::generate(ret.begin(), ret.end(), gen);

  return ret;
}

std::vector<uint8_t> sequential_data(size_t size) {
  std::vector<uint8_t> ret(size);

  for (size_t i = 0; i < size; ++i) {
    ret[i] = (uint8_t) (i & size_t(255));
  }

  return ret;
}
