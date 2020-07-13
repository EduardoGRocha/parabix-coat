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

# TODO Debug asmJIT

#-----------------------------------------------------------------------------------------------------------------------

add_executable(parabix_asmjit main.cpp)
if(ENABLE_ASMJIT)
    target_compile_definitions(parabix_asmjit PRIVATE "ENABLE_ASMJIT")
    target_link_libraries(parabix_asmjit ${ASMJIT_LIBRARIES})
endif()
