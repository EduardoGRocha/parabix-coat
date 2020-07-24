#include <chrono>
#include <vector>

#include <coat/Function.h>
#include <coat/ControlFlow.h>
#include <coat/Ptr.h>

//#include "scalar/character_database_64.h"
#include "scalar/operations_64_coat.h"
#include "utils.h"

using compiled_f_t = void (*) (uint64_t*, uint64_t*, size_t);

inline ValueAsmJit match_0_9(const ValueAsmJit &B7, const ValueAsmJit &B6, const ValueAsmJit &B5,
                        const ValueAsmJit &B4, const ValueAsmJit &B3, const ValueAsmJit &B2, const ValueAsmJit &B1, const ValueAsmJit &B0) {
  ValueAsmJit A1 = ~(B7 | B6);
  ValueAsmJit A2 = ~(B5 | B4);
  ValueAsmJit A3 = ~(B3 & B2);
  ValueAsmJit A4 = ~(B3 & B1);

  ValueAsmJit A5 = A1 & A2;
  ValueAsmJit A6 = A3 & A4;

  ValueAsmJit ret = A5 & A6;
  return ret;
}

int main(int argc, char* argv[]) {
#if ENABLE_ASMJIT
  coat::runtimeasmjit asmrt;
  coat::Function<coat::runtimeasmjit,compiled_f_t> fn(asmrt, "f");
  using Value = ValueAsmJit;
#endif
// TODO uncomment me for LLVM
//#ifdef ENABLE_LLVMJIT
//#define IS_LLVM
//  coat::runtimellvmjit::initTarget();
//  coat::runtimellvmjit llvmrt;
//  llvmrt.setOptLevel(3);
//  using CC = llvm::IRBuilder<>;
//  coat::Function<coat::runtimellvmjit,compiled_f_t> fn(llvmrt, "f");
//  using Value = ValueLLVM;
//#endif
  auto compilation_start = std::chrono::high_resolution_clock::now();
  {
    // get function arguments as "meta-variables"
    auto args = fn.getArguments("input", "output", "size");
    auto &input = std::get<0>(args);
    auto &output = std::get<1>(args);
    auto &size = std::get<2>(args);
    Value offset(fn, uint64_t(0), "offset");

    auto B7_ptr = input;
    offset += size;
    auto B6_ptr = input + offset;
    offset += size;
    auto B5_ptr = input + offset;
    offset += size;
    auto B4_ptr = input + offset;
    offset += size;
    auto B3_ptr = input + offset;
    offset += size;
    auto B2_ptr = input + offset;
    offset += size;
    auto B1_ptr = input + offset;
    offset += size;
    auto B0_ptr = input + offset;

    Value count(fn, uint64_t(0), "count");

    Value carry_advance_130(fn, uint64_t(0), "carry_advance_130");
    Value carry_match_star(fn, uint64_t(0), "carry_match_star");
    Value carry_advance_140(fn, uint64_t(0), "carry_advance_140");

    coat::loop_while(fn, count < size, [&]{
      auto B0 = *(B0_ptr + count);
      auto B1 = *(B1_ptr + count);
      auto B2 = *(B2_ptr + count);
      auto B3 = *(B3_ptr + count);
      auto B4 = *(B4_ptr + count);
      auto B5 = *(B5_ptr + count);
      auto B6 = *(B6_ptr + count);
      auto B7 = *(B7_ptr + count);
//      auto B0 = *B0_ptr;
//      auto B1 = *B1_ptr;
//      auto B2 = *B2_ptr;
//      auto B3 = *B3_ptr;
//      auto B4 = *B4_ptr;
//      auto B5 = *B5_ptr;
//      auto B6 = *B6_ptr;
//      auto B7 = *B7_ptr;

      auto matched_130 = match_130<Value>(B7, B6, B5, B4, B3, B2, B1, B0);
      auto matched_0_9 = match_0_9(B7, B6, B5, B4, B3, B2, B1, B0);
      auto matched_140 = match_140<Value>(B7, B6, B5, B4, B3, B2, B1, B0);

      auto r_advanced_130 = advance<Value>(matched_130, carry_advance_130);
      auto advanced_130 = r_advanced_130.result;
      carry_advance_130 = r_advanced_130.carry;

      auto r_match_star = match_star<Value>(advanced_130, matched_0_9, carry_match_star);
      auto matched_star = r_match_star.result;
      carry_match_star = r_match_star.carry;

      auto r_advanced_140 = advance<Value>(matched_star & matched_140, carry_advance_140);
      *output = r_advanced_140.result;
      carry_advance_140 = r_advanced_140.carry;

      ++output;
      count += 1;

//      ++B0_ptr;
//      ++B1_ptr;
//      ++B2_ptr;
//      ++B3_ptr;
//      ++B4_ptr;
//      ++B5_ptr;
//      ++B6_ptr;
//      ++B7_ptr;
    });
    // LLVM needs this ret statement
    coat::ret(fn);
  }

#ifdef IS_LLVM
  if(llvmrt.getOptLevel() > 0){
    llvmrt.optimize();
    llvmrt.print("jit3_opt.ll");
    if(!llvmrt.verifyFunctions()){
      puts("verification after optimization failed. aborting.");
      exit(EXIT_FAILURE); //FIXME: better error handling
    }
  }
#endif
  compiled_f_t f = fn.finalize();
  auto compilation_end = std::chrono::high_resolution_clock::now();
  auto elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>(compilation_end - compilation_start).count();
  std::cout << "Compilation time: " << elapsed_seconds << "\n";

//  std::vector<uint8_t> input = small_test_sequence();
  std::vector<uint8_t> input = random_byte_data(256*1024*1024);
  auto N_VECTORS = input.size()/VECTOR_LENGTH;
  auto N_ROWS = input.size();

  auto output = std::vector<uint8_t>(N_ROWS, 0);
  auto output_stream = std::vector<uint64_t>(N_ROWS, 0);

  transpose_sse(input.data(), output.data(), N_ROWS, 8);

  uint8_t state0[256];
  uint8_t state1[256];
  uint8_t state2[256];
  uint8_t state3[256];

  for (size_t i = 0; i < 256; ++i) {
    state0[i] = 0;
    state1[i] = 0;
    state2[i] = 0;
    state3[i] = 0;

    if (i <= 9) {
      state1[i] = 2;
      state2[i] = 2;
    }
    else if (i == 130) {
      state0[i] = 1;
    }
    else if (i == 140) {
      state2[i] = 3;
    }
  }

  std::vector<uint8_t*> states = {
      state0, state1, state2, state3
  };

  PerfEvent e;
  e.startCounters();
  auto execution_start = std::chrono::high_resolution_clock::now();
//  uint8_t cur_state = 0;
//  for (const auto& i : input) {
//    cur_state = states[cur_state][i];
//  }
  f(reinterpret_cast<uint64_t*>(output.data()),
      output_stream.data(), size_t(N_VECTORS));
  e.stopCounters();
  e.printReport(std::cout, 1); // use n as scale factor
  std::cout << std::endl;
//  std::cout << cur_state;
  auto execution_end = std::chrono::high_resolution_clock::now();
  auto elapsed_execution = std::chrono::duration_cast<std::chrono::microseconds>(execution_end - execution_start).count();
  std::cout << "Execution time: " << elapsed_execution << "\n";

  auto stream_7 = reinterpret_cast<uint64_t *>(output.data());
  auto stream_6 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS;
  auto stream_5 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 2;
  auto stream_4 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 3;
  auto stream_3 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 4;
  auto stream_2 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 5;
  auto stream_1 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 6;
  auto stream_0 = reinterpret_cast<uint64_t *>(output.data()) + N_VECTORS * 7;

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
}
