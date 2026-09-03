#!/bin/bash -eu
# ============================================================
# .clusterfuzzlite/build.sh
#
# VectorPro is header-only, so unlike a harness that needs to compile
# separate .cpp translation units first, this just compiles the fuzz
# target directly against the headers under include/.
#
# Add more `${SRC}/VectorPro/fuzz/fuzz_*.cpp` harnesses here as
# they're added; each becomes its own $OUT binary.
# ============================================================

cd "${SRC}/VectorPro"

$CXX $CXXFLAGS -std=c++20 \
  -I"${SRC}/VectorPro/include" \
  fuzz/fuzz_vector.cpp \
  $LIB_FUZZING_ENGINE \
  -o "${OUT}/fuzz_vector"
