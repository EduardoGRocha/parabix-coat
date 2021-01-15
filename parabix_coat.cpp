#include <chrono>
#include <vector>

#include "perfevent/PerfEvent.hpp"
#include <coat/Function.h>
#include <coat/ControlFlow.h>
#include <coat/Ptr.h>

#include "scalar/character_database_64.h"
#include "scalar/operations_64_coat.h"
#include "utils.h"

using compiled_f_t = void (*) (uint64_t*, uint64_t*, size_t);

template<typename Value, typename Fn>
void get_compiled_function(Fn &fn) {
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

      auto matched_130 = match_130<Value>(B7, B6, B5, B4, B3, B2, B1, B0);
      auto matched_0_9 = match_0_9<Value>(B7, B6, B5, B4, B3, B2, B1, B0);
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
    });
    // LLVM needs this ret statement
    coat::ret(fn);
  }

}

void benchmark_compile_function_llvm(size_t opt_level) {
  size_t n_iterations = 200;
  auto compilation_start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < n_iterations; ++i) {
    coat::runtimellvmjit::initTarget();
    coat::runtimellvmjit llvmrt;
    llvmrt.setOptLevel(opt_level);
    using CC = llvm::IRBuilder<>;
    coat::Function<coat::runtimellvmjit,compiled_f_t> fn(llvmrt, "f");
    using Value = ValueLLVM;
    get_compiled_function<Value>(fn);

    if(llvmrt.getOptLevel() > 0){
      llvmrt.optimize();
//    Debug
//    llvmrt.print("jit3_opt.ll");
    }
    compiled_f_t f = fn.finalize();
  }
  auto compilation_end = std::chrono::high_resolution_clock::now();
  auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(compilation_end - compilation_start).count();
  std::cout << "Elapsed LLVM(" << opt_level << "):" << elapsed_us/n_iterations << "\n";
}

void benchmark_compile_function_asmjit() {
  // Measures in ms
  size_t n_iterations = 200;
  auto compilation_start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < n_iterations; ++i) {
    coat::runtimeasmjit asmrt;
    coat::Function<coat::runtimeasmjit,compiled_f_t> fn(asmrt, "f");
    using Value = ValueAsmJit;
    get_compiled_function<Value>(fn);
    compiled_f_t f = fn.finalize();
  }
  auto compilation_end = std::chrono::high_resolution_clock::now();
  auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(compilation_end - compilation_start).count();
  std::cout << "Elapsed asmjit: " << elapsed_us/n_iterations << "\n";
}

void benchmark_llvm(uint64_t *input, uint64_t *output_stream, size_t opt_level) {
  coat::runtimellvmjit::initTarget();
  coat::runtimellvmjit llvmrt;
  llvmrt.setOptLevel(opt_level);
  using CC = llvm::IRBuilder<>;
  coat::Function<coat::runtimellvmjit,compiled_f_t> fn(llvmrt, "f");
  using Value = ValueLLVM;
  get_compiled_function<Value>(fn);

  if(llvmrt.getOptLevel() > 0){
    llvmrt.optimize();
//    Debug
//    llvmrt.print("jit3_opt.ll");
  }
  compiled_f_t f = fn.finalize();

  std::vector<size_t> workload_sizes = { 256*1024*1024 };

  for (const auto& workload_size : workload_sizes) {
    size_t N_VECTORS = workload_size/64;
    size_t n_iterations = 20;
    PerfEvent e;
    e.startCounters();
    for (size_t iteration = 0; iteration < n_iterations; ++iteration) {
      f(input, output_stream, N_VECTORS);
    }
    e.stopCounters();
    // Normalize per Byte
    e.printReport(std::cout, n_iterations*workload_size); // use n as scale factor
    std::cout << std::endl;
  }
}

void benchmark_asmjit(uint64_t *input, uint64_t *output_stream) {
  coat::runtimeasmjit asmrt;
  coat::Function<coat::runtimeasmjit,compiled_f_t> fn(asmrt, "f");
  using Value = ValueAsmJit;
  get_compiled_function<Value>(fn);
  compiled_f_t f = fn.finalize();

  std::vector<size_t> workload_sizes = { 256*1024*1024 };

  for (const auto& workload_size : workload_sizes) {
    size_t N_VECTORS = workload_size/64;
    size_t n_iterations = 20;
    PerfEvent e;
    e.startCounters();
    for (size_t iteration = 0; iteration < n_iterations; ++iteration) {
      f(input, output_stream, N_VECTORS);
    }
    e.stopCounters();
    // Normalize per Byte
    e.printReport(std::cout, n_iterations*workload_size); // use n as scale factor
    std::cout << std::endl;
  }
}

void test(uint64_t *input, uint64_t *output_stream, size_t N_ROWS, bool asmjit) {
  size_t N_VECTORS = N_ROWS/64;
//  Print test
  auto stream_7 = input;
  auto stream_6 = input + N_VECTORS;
  auto stream_5 = input + N_VECTORS * 2;
  auto stream_4 = input + N_VECTORS * 3;
  auto stream_3 = input + N_VECTORS * 4;
  auto stream_2 = input + N_VECTORS * 5;
  auto stream_1 = input + N_VECTORS * 6;
  auto stream_0 = input + N_VECTORS * 7;

  if ( asmjit ) {
    // asmJIT
    coat::runtimeasmjit asmrt;
    coat::Function<coat::runtimeasmjit,compiled_f_t> fn(asmrt, "f");
    using Value = ValueAsmJit;
    get_compiled_function<Value>(fn);
    compiled_f_t f = fn.finalize();
    f(input, output_stream, N_VECTORS);
  } else {
    // LLVM
    coat::runtimellvmjit::initTarget();
    coat::runtimellvmjit llvmrt;
    llvmrt.setOptLevel(3);
    using CC = llvm::IRBuilder<>;
    coat::Function<coat::runtimellvmjit,compiled_f_t> fn(llvmrt, "f");
    using Value = ValueLLVM;
    get_compiled_function<Value>(fn);

    if(llvmrt.getOptLevel() > 0){
      llvmrt.optimize();
  //    Debug
  //    llvmrt.print("jit3_opt.ll");
    }
    compiled_f_t f = fn.finalize();
    f(input, output_stream, N_VECTORS);
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

  print_bit_array(output_stream, N_VECTORS);
}


int main(int argc, char* argv[]) {
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

  benchmark_asmjit(transposed_input_64, output_stream.data());

  line_break();

  benchmark_llvm(transposed_input_64, output_stream.data(), 0);
  line_break();
  benchmark_llvm(transposed_input_64, output_stream.data(), 1);
  line_break();
  benchmark_llvm(transposed_input_64, output_stream.data(), 2);
  line_break();
  benchmark_llvm(transposed_input_64, output_stream.data(), 3);
#else
  /// Benchmark compilation
  benchmark_compile_function_asmjit();
  benchmark_compile_function_llvm(0);
  benchmark_compile_function_llvm(1);
  benchmark_compile_function_llvm(2);
  benchmark_compile_function_llvm(3);
#endif

  // Matches a 1 at position AFTER occurrences
  input = small_test_sequence();
  N_ROWS = input.size();

  transposed_input = std::vector<uint8_t>(N_ROWS, 0);
  output_stream = std::vector<uint64_t>(N_ROWS, 0);

  transpose_sse(input.data(), transposed_input.data(), N_ROWS, 8);
  transposed_input_64 = reinterpret_cast<uint64_t*>(transposed_input.data());

  test(transposed_input_64, output_stream.data(), N_ROWS, true);
  test(transposed_input_64, output_stream.data(), N_ROWS, false);
}
