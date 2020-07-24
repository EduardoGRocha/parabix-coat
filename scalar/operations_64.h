#pragma once

#include <limits>

#ifndef MAX_UINT64
  #define MAX_UINT64 std::numeric_limits<uint64_t>::max()
#endif

struct ResultCarry {
  uint64_t result;
  uint64_t carry;
};

inline ResultCarry scan_thru(uint64_t M1, uint64_t CC, uint64_t carry_queue) {
  // Add operations require a carry queue, which require a triple sum (A + B + carry)
  // Since each of the individual sums might fail, split it into 2 sums and check for carry of each one
  // Since carry_queue = 0 or 1, the final carry queue is either 0 or 1, and can be OR-ed together
  // Also notice that the subtraction can't be negative since we are using 0xFFFFFFFF_FFFFFFFF as value
  uint64_t carry_M2 = (M1 > MAX_UINT64 - CC);
  uint64_t M2 = M1 + CC;
  uint64_t carry_M2_plus_carry = (carry_queue > MAX_UINT64 - M2);
  M2 = carry_queue + M2;
  uint64_t M3 = M2 & ~CC;
  return {M3, carry_M2_plus_carry | carry_M2};
}

inline ResultCarry match_star(uint64_t M1, uint64_t D, uint64_t carry_queue) {
  // Add operations require a carry queue, which require a triple sum (A + B + carry)
  // Since each of the individual sums might fail, split it into 2 sums and check for carry of each one
  // Since carry_queue = 0 or 1, the final carry queue is either 0 or 1, and can be OR-ed together
  // Also notice that the subtraction can't be negative since we are using 0xFFFFFFFF_FFFFFFFF as value
  uint64_t T0 = M1 & D;
  uint64_t carry_T1 = (D > MAX_UINT64 - T0);
  uint64_t T1 = T0 + D;
  uint64_t carry_T1_plus_carry = (carry_queue > MAX_UINT64 - T1);
  T1 = carry_queue + T1;
  uint64_t T2 = T1 ^ D;
  uint64_t M2 = T2 | M1;

  return {M2, (carry_T1 | carry_T1_plus_carry)};
}

inline ResultCarry advance(uint64_t A, uint64_t carry_queue) {
  uint64_t result = A << 1;
  uint64_t carry = A >> 63;
  return {(result | carry_queue), carry};
}
