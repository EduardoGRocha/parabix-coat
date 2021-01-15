#About
This project is based on the thesis "High-Performance Regular Expression Matching with Parabix and LLVM"
which can be found [here](https://www.semanticscholar.org/paper/High-Performance-Regular-Expression-Matching-with-Denis/d4cb59d780f93f308c8591bda49cb766db24c520). 
The main idea is to replace the "slow" LLVM JIT compilation framework with 
a low-latency backend that directly emits x86 instructions

Code for transposing was based on [here](https://mischasan.wordpress.com/2011/10/03/the-full-sse2-bit-matrix-transpose-routine/)
Even though the repository doesn't officially have a license, 
I follow the author's choice of GPLv2 until I take the time to write this 
subroutine myself.

#Running

To compile just run ./build_dependencies.sh -j NThreads
