cmake_minimum_required(VERSION 3.16.3)
project(testHOG)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_EXE_LINKER_FLAGS -ldl)

set(EXECUTABLE_OUTPUT_PATH ${testHOG_SOURCE_DIR}/bin)

include_directories(./include)
include_directories(./malloc_count)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -DDEBUG")

add_library(COMMON_LIBRARY ./src/Aho-Cora.cpp ./src/EHOG.cpp ./src/timer.cpp ./src/DatasetGenerator.cpp ./malloc_count/malloc_count.c)

# Correctness driver (input-mode independent).
add_executable(TEST ./src/HOG-SSP.cpp ./tests/validateHOG.cpp)
target_compile_definitions(TEST PUBLIC SSP)
target_link_libraries(TEST PUBLIC COMMON_LIBRARY)

# ---------------------------------------------------------------------------
# Construction benchmarks.
#
# Every binary is built from tests/testHOG.cpp. Two axes of configuration:
#   * algorithm / stage  -> algorithm .cpp + macro (SSP/SK/SP/EC/BCER,
#                           CONSTRUCT_AHO/CONSTRUCT_EHOG, VIA_EHOG)
#   * INPUT MODE         -> INPUT_RANDOM | INPUT_READ_RANDOM | INPUT_REAL,
#                           which selects argv parsing + dataset generator.
#
# The input mode is a compile-time macro, so each experiment gets its own set
# of binaries with a name suffix:
#   _R    -> random strings        (scripts/test_random.sh:        k n seed)
#   _RR   -> random reads          (scripts/test_read_random.sh:   complete snapshot coverage seed)
#   _REAL -> real datasets         (scripts/script*.sh:            dataset_name)
# ---------------------------------------------------------------------------

function(add_hog_target name src drv)
  add_executable(${name} ${src} ${drv})
  target_link_libraries(${name} PUBLIC COMMON_LIBRARY)
  target_compile_definitions(${name} PUBLIC ${ARGN})
endfunction()

# One full algorithm set for a given (suffix, input-mode-macro).
function(add_hog_mode suffix mode)
  add_hog_target(AHO${suffix}      ./src/HOG-SSP.cpp  ./tests/testHOG.cpp CONSTRUCT_AHO  ${mode})
  add_hog_target(EHOG${suffix}     ./src/HOG-SSP.cpp  ./tests/testHOG.cpp CONSTRUCT_EHOG ${mode})

  add_hog_target(SSP${suffix}      ./src/HOG-SSP.cpp  ./tests/testHOG.cpp SSP  ${mode})
  add_hog_target(SK${suffix}       ./src/HOG-SK.cpp   ./tests/testHOG.cpp SK   ${mode})
  add_hog_target(SP${suffix}       ./src/HOG-SP.cpp   ./tests/testHOG.cpp SP   ${mode})
  add_hog_target(EC${suffix}       ./src/HOG-EC.cpp   ./tests/testHOG.cpp EC   ${mode})
  add_hog_target(BCER${suffix}     ./src/HOG-BCER.cpp ./tests/testHOG.cpp BCER ${mode})

  add_hog_target(SSPEHOG${suffix}  ./src/HOG-SSP.cpp  ./tests/testHOG.cpp SSP  VIA_EHOG ${mode})
  add_hog_target(SKEHOG${suffix}   ./src/HOG-SK.cpp   ./tests/testHOG.cpp SK   VIA_EHOG ${mode})
  add_hog_target(SPEHOG${suffix}   ./src/HOG-SP.cpp   ./tests/testHOG.cpp SP   VIA_EHOG ${mode})
  add_hog_target(ECEHOG${suffix}   ./src/HOG-EC.cpp   ./tests/testHOG.cpp EC   VIA_EHOG ${mode})
  add_hog_target(BCEREHOG${suffix} ./src/HOG-BCER.cpp ./tests/testHOG.cpp BCER VIA_EHOG ${mode})
endfunction()

add_hog_mode(_R    INPUT_RANDOM)
add_hog_mode(_RR   INPUT_READ_RANDOM)
add_hog_mode(_REAL INPUT_REAL)

# ---------------------------------------------------------------------------
# apsphog: all-pairs suffix-prefix HOG library (previously a separate CMake
# project under apsphog/). Folded in here so a single top-level build produces
# it too. Header-only consumers include apsphog/include/Hog.h directly; this
# library provides the compiled Hog.cpp for anything that links it.
# ---------------------------------------------------------------------------
add_library(apsphog ./apsphog/src/Hog.cpp)
target_include_directories(apsphog PUBLIC ./apsphog/include ./include)

# ---------------------------------------------------------------------------
# HOGQueries: HOG-only query correctness + benchmark (tests/testQueriesHOG.cpp).
# Uses the query-oriented "x" stack: HOG-SKx (+ the reconstructed Aho-Corax
# trie) builds an apsphog Hog via make_hog(), then runs the five query types.
# main() first runs test_validity_queries() (assertions on a 3-string example),
# then benchmarks the datasets in data/ (repo root). Run from the repository
# root so data/ resolves.
# ---------------------------------------------------------------------------
add_executable(HOGQueries ./tests/testQueriesHOG.cpp ./src/HOG-SKx.cpp
                          ./src/Aho-Corax.cpp ./src/timer.cpp)
target_include_directories(HOGQueries PUBLIC ./include ./apsphog/include)
target_link_libraries(HOGQueries PUBLIC apsphog)

# ---------------------------------------------------------------------------
# KMPvsHOG: HOG-vs-KMP query benchmark (tests/testQueriesKMP.cpp).
# Runs the five query types for both HOG and a KMP baseline over each real
# dataset and prints ready-to-paste LaTeX tables to stdout.
# Reads datasets from data/ (repo root), so run it from the repository root
# (as scripts/test_Queries_KMP.sh does).
# ---------------------------------------------------------------------------
add_executable(KMPvsHOG ./tests/testQueriesKMP.cpp ./src/timer.cpp)
target_include_directories(KMPvsHOG PUBLIC ./include)

