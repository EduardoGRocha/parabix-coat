cmake_minimum_required(VERSION 3.16)
project(parabix_asmjit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For LLVM needs to compile with -rdynamic -fno-rtti
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wextra -march=native -fno-rtti -rdynamic -Wl,--no-as-needed")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -march=native -fno-rtti -rdynamic -Wl,--no-as-needed")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

#-----------------------------------------------------------------------------------------------------------------------
# Adding asmJIT
option(ENABLE_ASMJIT "enable AsmJit backend" ON)

if(ENABLE_ASMJIT)
    # find asmjit
    find_path(ASMJIT_INCLUDE_DIR
            asmjit/asmjit.h
            HINTS external/asmjit ENV ASMJIT_ROOT
            PATH_SUFFIXES src
            )
    find_library(ASMJIT_LIBRARIES
            asmjit
            HINTS external/asmjit ENV ASMJIT_ROOT
            PATH_SUFFIXES build build_next
            )
    include_directories(${ASMJIT_INCLUDE_DIR})
endif()

# find asmjit_perf
find_path(ASMJIT_PERF_INCLUDE_DIR
        asmjit-utilities/perf/jitdump.h
        HINTS external/coat/external
        )
find_library(ASMJIT_PERF_LIBRARIES
        asmjit_perf
        HINTS external/asmjit-utilities
        PATH_SUFFIXES perf/build
        )
include_directories(${ASMJIT_PERF_INCLUDE_DIR})

# find llvm, quick&dirty
find_path(LLVM_INCLUDE_DIR
        llvm/IR/IRBuilder.h
        HINTS external/llvm/llvm/install ENV LLVM_ROOT
        PATH_SUFFIXES include
        )
find_library(LLVM_LIBRARIES
        LLVM-7.1
        HINTS external/llvm/llvm/install ENV LLVM_ROOT
        PATH_SUFFIXES lib build/lib
        )
include_directories(${LLVM_INCLUDE_DIR})

# debugging options
if(NOT DEFINED PROFILING)
    set(PROFILING "" CACHE STRING "Choose type of profiling support for perf and AsmJit, options are: None, Assembly, Source")
endif()

if(PROFILING STREQUAL "Assembly")
    add_compile_definitions("PROFILING_ASSEMBLY")
elseif(PROFILING STREQUAL "Source")
    add_compile_definitions("PROFILING_SOURCE")
endif()

#-----------------------------------------------------------------------------------------------------------------------

include_directories("${CMAKE_SOURCE_DIR}/external/coat/include/")

add_executable(parabix_cpp parabix_cpp.cpp)
add_executable(asmjit_tryout asmjit_tryout.cpp)
add_executable(parabix_coat parabix_coat.cpp)
add_executable(DFA DFA.cpp)

if(ENABLE_ASMJIT)
    target_compile_definitions(parabix_cpp PRIVATE "ENABLE_ASMJIT")
    target_link_libraries(parabix_cpp ${ASMJIT_LIBRARIES})
    target_compile_definitions(asmjit_tryout PRIVATE "ENABLE_ASMJIT")
    target_link_libraries(asmjit_tryout ${ASMJIT_LIBRARIES})
    target_compile_definitions(parabix_coat PRIVATE "ENABLE_ASMJIT" PRIVATE "ENABLE_LLVMJIT")
    target_link_libraries(parabix_coat ${ASMJIT_LIBRARIES} ${LLVM_LIBRARIES})
    target_link_libraries(DFA)
endif()
