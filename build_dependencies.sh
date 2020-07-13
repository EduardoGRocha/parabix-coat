#!/bin/bash
# From coat's repo

git submodule update --init

pushd external/asmjit
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make $@
popd
