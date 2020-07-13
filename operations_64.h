#pragma once

inline uint64_t scan_through(uint64_t M1, uint64_t CC) {
  uint64_t M2 = M1 + CC;
  uint64_t M3 = M2 & ~CC;
  return M3;
}

inline uint64_t match_star(uint64_t M1, uint64_t D) {
  uint64_t T0 = M1 & D;
  uint64_t T1 = T0 + D;
  uint64_t T2 = T1 ^ D;
  uint64_t M2 = T2 | M1;

  return M2;
}

inline uint64_t advance(uint64_t A) {
  return A << 1;
}
