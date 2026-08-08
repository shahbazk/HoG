#!/bin/bash
# HOG-only query correctness + benchmark.
# Builds and runs HOGQueries (tests/testQueriesHOG.cpp), which uses the "x" query
# stack (HOG-SKx + Aho-Corax -> apsphog Hog). It first runs the validity
# assertions on a small example, then benchmarks the five query types over the
# datasets in data/ (repo root).
#
# Run from the repository root.

set -u

cmake -B build
cmake --build build -j"$(nproc)" --target HOGQueries

./bin/HOGQueries
