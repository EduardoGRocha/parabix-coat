cmake_minimum_required(VERSION 3.16)
project(parabix_asmjit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_executable(parabix_asmjit main.cpp)
add_executable(asmjit_tryout asmjit_tryout.cpp)

if(ENABLE_ASMJIT)
    target_compile_definitions(parabix_asmjit PRIVATE "ENABLE_ASMJIT")
    target_link_libraries(parabix_asmjit ${ASMJIT_LIBRARIES})
    target_compile_definitions(asmjit_tryout PRIVATE "ENABLE_ASMJIT")
    target_link_libraries(asmjit_tryout ${ASMJIT_LIBRARIES})
endif()
