#pragma once

#include <limits>

using ValueAsmJit = coat::Value<::asmjit::x86::Compiler, uint64_t>;
using ValueLLVM = coat::Value<::llvm::IRBuilder<>, uint64_t>;

template<typename Vector>
struct ResultCarry {
  Vector result;
  Vector carry;
};

// Coat names internal register differently for LLVM and AsmJIT, so need 2 diff carry functions

// Carry flag for A + B with asmjit
inline ValueAsmJit carry_condition(const ValueAsmJit &A, const ValueAsmJit &B) {
  auto MAX_COAT_T = ValueAsmJit(A.cc, uint64_t(std::numeric_limits<uint64_t>::max()));

  auto condition_carry_M2 = (A > MAX_COAT_T - B);
  condition_carry_M2.compare();
  ValueAsmJit carry_M2(A.cc);
  carry_M2.reg = condition_carry_M2.reg;
  return carry_M2;
}

// Carry flag for A + B
inline ValueLLVM carry_condition(const ValueLLVM &A, const ValueLLVM &B) {
  auto MAX_COAT_T = ValueLLVM(A.cc, uint64_t(std::numeric_limits<uint64_t>::max()));

  auto condition_carry_M2 = (A > MAX_COAT_T - B);
  condition_carry_M2.compare();
  ValueLLVM carry_M2(A.cc);
  carry_M2.memreg = condition_carry_M2.reg;
  return carry_M2;
}

template<typename Vector, typename T=uint64_t>
inline ResultCarry<Vector> scan_thru(const Vector &M1, const Vector &CC, const Vector &carry_queue) {
  // Add operations require a carry queue, which require a triple sum (A + B + carry)
  // Since each of the individual sums might fail, split it into 2 sums and check for carry of each one
  // Since carry_queue = 0 or 1, the final carry queue is either 0 or 1, and can be OR-ed together
  // Also notice that the subtraction can't be negative since we are using 0xFFFFFFFF_FFFFFFFF as value
  auto carry_M2 = carry_condition(M1, CC);
  auto M2 = M1 + CC;
  auto carry_M2_plus_carry = carry_condition(carry_queue, M2);
  M2 = carry_queue + M2;
  auto M3 = M2 & ~CC;
  return {M3, carry_M2_plus_carry | carry_M2};
}

template<typename Vector, typename T=uint64_t>
inline ResultCarry<Vector> match_star(const Vector &M1, const Vector &D, const Vector &carry_queue) {
  // Add operations require a carry queue, which require a triple sum (A + B + carry)
  // Since each of the individual sums might fail, split it into 2 sums and check for carry of each one
  // Since carry_queue = 0 or 1, the final carry queue is either 0 or 1, and can be OR-ed together
  // Also notice that the subtraction can't be negative since we are using 0xFFFFFFFF_FFFFFFFF as value
  auto T0 = M1 & D;
  auto carry_T1 = carry_condition(T0, D);
  auto T1 = T0 + D;

  auto carry_T1_plus_carry = carry_condition(carry_queue, T1);
  T1 = carry_queue + T1;

  auto T2 = T1 ^ D;
  auto M2 = T2 | M1;

  return {M2, (carry_T1 | carry_T1_plus_carry)};
}

template<typename Vector>
inline ResultCarry<Vector> advance(const Vector &A, const Vector &carry_queue) {
  Vector result = A << 1;
  Vector carry = A >> 63;
  return {(result | carry_queue), carry};
}
