#include <asmjit/asmjit.h>
#include <cstdio>
#include <iostream>
#include <chrono>

using namespace asmjit;
// Signature of the generated function
typedef void (*MemCpy64)(uint64_t* dst_ptr, uint64_t* src_ptr, size_t count);


int main(int argc, char* argv[]) {
  // Runtime specialized for JIT code execution.
  JitRuntime jit;
  // Holds code and relocation information.
  CodeHolder code;

  // Initialize to the same arch as JIT runtime.
  code.init(jit.codeInfo());

  // Log generated Assembly
//  FileLogger logger(stdout);
//  code.setLogger(&logger);

  // Create and attach x86::Compiler to `code`.
  x86::Compiler cc(&code);

  // Begin the function of the following signature:
  //   Return value - void      (no return value).
  //   1st argument - uint64_t* (machine reg-size).
  //   2nd argument - uint64_t* (machine reg-size).
  //   3rd argument - size_t    (machine reg-size).
  auto compilation_start = std::chrono::high_resolution_clock::now();
  cc.addFunc(FuncSignatureT<void, uint32_t**, uint32_t**, size_t>());

  // Start of the loop.
  Label L_Loop = cc.newLabel();
  // Used to exit early.
  Label L_Exit = cc.newLabel();

  // Create `dst` register (destination pointer).
  x86::Gp dst = cc.newIntPtr("dst_ptr");
  // Create `src` register (source pointer).
  x86::Gp src = cc.newIntPtr("src_ptr");
  // Create `cnt` register (loop counter).
  x86::Gp cnt = cc.newUIntPtr("cnt");

  // Assign `dst` argument.
  cc.setArg(0, dst);
  // Assign `src` argument.
  cc.setArg(1, src);
  // Assign `cnt` argument.
  cc.setArg(2, cnt);

  // Early exit if length is zero.
  cc.test(cnt, cnt);
  cc.jz(L_Exit);

  // Bind the beginning of the loop here.
  cc.bind(L_Loop);

  // Copy a single dword (4 bytes).
  x86::Gp tmp = cc.newUInt64("tmp");
  // Load QWORD from [src] address.
  cc.mov(tmp, x86::qword_ptr(src));
  // Store QWORD to [dst] address.
  cc.mov(x86::qword_ptr(dst), tmp);

  // Increment `src`.
  cc.add(src, 8);
  // Increment `dst`.
  cc.add(dst, 8);

  // Loop until `cnt` is non-zero.
  cc.dec(cnt);
  cc.jnz(L_Loop);

  // Label used by early exit.
  cc.bind(L_Exit);
  // End of the function body.
  cc.endFunc();

  // Translate and assemble the whole `cc` content.
  cc.finalize();
  // ----> x86::Compiler is no longer needed from here and can be destroyed <----

  MemCpy64 mem_cpy_64;
  // Add the generated code to the runtime.
  Error err = jit.add(&mem_cpy_64, &code);
  // Handle a possible error returned by AsmJit.
  auto compilation_end = std::chrono::high_resolution_clock::now();
  auto elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>(compilation_end - compilation_start).count();
  std::cout << elapsed_seconds << "\n";

  if (err) return 1;
  // ----> CodeHolder is no longer needed from here and can be destroyed <----

  size_t data_size = 10;
  auto* data_src = (uint64_t*) malloc(data_size*sizeof(uint64_t));
  auto* data_dst = (uint64_t*) malloc(data_size*sizeof(uint64_t));
  for (uint64_t i = 0; i < data_size; ++i) {
      data_src[i] = i;
      data_dst[i] = 0;
  }
	
  uint64_t* input = data_src;
  uint64_t* output = data_dst;
  // Test the generated code.
  mem_cpy_64(output, input, data_size);

  for (uint64_t i = 0; i < data_size; i++) {
    printf("%lu\n", input[i]);
      printf("%lu\n", output[i]);
  }

  // RAII, but let's make it explicit.
  jit.release(mem_cpy_64);
  return 0;
}
