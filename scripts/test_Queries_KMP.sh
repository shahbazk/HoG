#!/bin/bash
# HOG-vs-KMP query benchmark.
# Builds and runs KMPvsHOG (tests/testQueriesKMP.cpp), which reads real
# datasets from data/ (repo root) and prints LaTeX tables (HOG times, KMP
# times, and KMP/HOG relative ratios) to stdout.
#
# Run from the repository root. Output is captured to queries_tables.tex.

set -u

cmake -B build
cmake --build build -j"$(nproc)" --target KMPvsHOG

./bin/KMPvsHOG > queries_tables.tex

echo "Wrote queries_tables.tex"
