# HoG — Hierarchical Overlap Graphs

Construction algorithms and query benchmarks for **Hierarchical Overlap Graphs (HOG)** and **Extended Hierarchical Overlap Graphs (EHOG)** over string sets, with comparisons against several baseline HOG-construction algorithms and a KMP-based query baseline.

The code accompanies experiments on:

- **HOG / EHOG construction** — timing and memory across multiple algorithm variants, on both randomly generated string sets and real genome-assembly read sets.
- **HOG queries** — five query types (*OneToOne*, *OneToAll*, *Top*, *Count*, *Report*) benchmarked against a KMP baseline.

---

## Repository layout

```
include/        Algorithm headers (HOG-SSP, HOG-SK, HOG-SP, HOG-BCER, HOG-EC, EHOG, Aho-Cora, ...)
src/            Implementations + DatasetGenerator + timer
tests/          Test / benchmark drivers
  testHOG.cpp         construction benchmark (built by CMake into several binaries)
  validateHOG.cpp     correctness checks for construction
  testQueries.cpp     HOG query benchmark  (+ validity assertions)
  testQueriesKMP.cpp  HOG-vs-KMP query benchmark; emits LaTeX tables
apsphog/        Standalone all-pairs-suffix-prefix HOG library (separate CMake project)
scripts/        build + experiment-runner + gnuplot plotting scripts
plots/          CSV outputs produced by the experiment scripts
graphs/         PNG figures produced by the gnuplot scripts
profiling/      valgrind / gprof / massif artefacts (reference only)
CMakeLists.txt  top-level build for the construction benchmarks
```

The `data/`, `bin/`, `build/`, `dump/`, `tmp/`, and `plots/` output paths are git-ignored — they are created at build/run time.

---

## Prerequisites

- **CMake ≥ 3.16.3** and a **C++20** compiler (g++ ≥ 10 or clang ≥ 11).
- **GNU `time`** (`/usr/bin/time`, not the shell builtin) — the runner scripts use its `-f`/`--output` flags for memory and CPU measurement. On Debian/Ubuntu: `sudo apt install time`.
- **gnuplot** — for regenerating the figures in `graphs/`.
- **`malloc_count`** — the top-level `CMakeLists.txt` links `./malloc_count/malloc_count.c` and adds `./malloc_count` to the include path, but this directory is **not** vendored in the repo. Before building, place Timo Bingmann's `malloc_count` (`malloc_count.c` / `malloc_count.h`) into a `malloc_count/` directory at the repo root. See <https://github.com/bingmann/malloc_count>.

---

## Building the construction benchmarks

From the repository root:

```bash
bash scripts/build.sh
```

which is simply:

```bash
cmake -B build
cmake --build build
```

Binaries are emitted to `./bin/`. The build produces one executable per algorithm/pipeline combination, including:

| Binary        | Algorithm / stage                                    |
|---------------|------------------------------------------------------|
| `AHO`         | Aho–Corasick trie construction (shared first stage)  |
| `EHOG`        | EHOG construction                                    |
| `SSP`, `SK`, `SP`, `EC`, `BCER` | HOG construction variants (built directly from the Aho–Corasick trie) |
| `SSPEHOG`, `SKEHOG`, `SPEHOG`, `ECEHOG`, `BCEREHOG` | same variants, built **via** the EHOG intermediate |
| `TEST`        | `validateHOG` correctness driver                     |

The `*EHOG` variants read the pre-built EHOG; the plain variants read the Aho–Corasick output. Because the stages share intermediate files under `dump/`, run `AHO` (and `EHOG`, for the `*EHOG` variants) **before** the HOG variant for a given input.

> **Selecting the input mode.** `tests/testHOG.cpp` supports three input modes — real datasets, random strings, and random reads — selected by (un)commenting the corresponding block near the top of `main()`. The repository ships with the **random-reads** mode active (`generate_random_read_data`, taking `complete_len snapshot_len coverage seed`). To benchmark real datasets or plain random strings instead, switch the active block and rebuild.

---

## Providing datasets

### Real genome assemblies

Real-data drivers look for files under **`data/`** (relative to the working directory; the query driver looks under `../data/` — see below). Each dataset file is plain text:

```
<k>
<string_1>
<string_2>
...
<string_k>
```

i.e. the number of strings on the first line, followed by one string per line. Dataset names used in the paper (place files with exactly these names in `data/`):

```
A_hydrophila_HiSeq   B_cereus_HiSeq   B_cereus_MiSeq   B_fragilis_HiSeq
M_abscessus_HiSeq    M_abscessus_MiSeq  R_sphaeroides_HiSeq  R_sphaeroides_MiSeq
S_aureus_HiSeq       V_cholerae_HiSeq   V_cholerae_MiSeq   X_axonopodis_HiSeq
```

(plus the plant assemblies `clementina`, `sinensis`, `trifoliata`, `elegans` used in some scripts).

### Synthetic data

No files are needed — synthetic inputs are generated in-process:

- **Random strings:** `./bin/<ALGO> <k> <n> <seed>` — `k` strings whose lengths sum to `n`.
- **Random reads:** `./bin/<ALGO> <complete_len> <snapshot_len> <coverage> <seed>` — `coverage`× reads of length `snapshot_len` sampled from a random base string of length `complete_len`.

Each run appends a CSV line (sizes, build time, and — via GNU `time` — peak memory) to the collecting output file.

---

## Running the construction experiments

Two turnkey runners build everything and sweep parameters, writing CSVs into `plots/`:

**Random strings** — sweeps `n` at fixed `k`, then `k` at fixed `n`, 20 trials each:

```bash
bash scripts/test_random.sh
# → plots/random_fixed_k , plots/random_fixed_n
```

**Random reads** — sweeps complete length × snapshot length × coverage:

```bash
bash scripts/test_read_random.sh
# → plots/read_random
```

**Real datasets** — edit the dataset list in `scripts/script_combined.sh` (or `script.sh`), place the corresponding files in `data/`, then:

```bash
bash scripts/script_combined.sh
# → per-algorithm timing/memory dumps under dump/
```

Both `test_*.sh` scripts call `scripts/build.sh` first, so a clean checkout can go straight to running them (once `malloc_count/` and `data/` are in place). Each algorithm is guarded by a per-run `timeout` (30 s for the sweeps, 1 h in `run.sh`); a timed-out cell is recorded as `timeout` in the CSV.

---

## Running the query benchmark (HOG vs KMP)

The query drivers are **not** part of the CMake build; compile them directly. They pull in the standalone-query header `HOG-SKx.h` and read datasets from `../data/`, so run them from inside `tests/`:

```bash
cd tests
g++ -O3 -std=c++20 -I../include testQueriesKMP.cpp -o testQueriesKMP
./testQueriesKMP        # expects datasets in ../data/
```

`testQueriesKMP` runs the five query types (*OneToOne*, *OneToAll*, *Top*, *Count*, *Report*) for both HOG and a KMP baseline over each dataset, and prints **ready-to-paste LaTeX tables** to stdout — absolute HOG times, absolute KMP times, and the KMP/HOG relative ratios. Redirect to capture:

```bash
./testQueriesKMP > queries_tables.tex
```

For a correctness check on a tiny hand-verified instance, `testQueries.cpp` runs `test_validity_queries()` (assertions on a 3-string example) before the real-data benchmark:

```bash
g++ -O3 -std=c++20 -I../include testQueries.cpp -o testQueries
./testQueries
```

---

## Regenerating the figures

The files in `scripts/plot_*` are gnuplot scripts that read the CSVs in `plots/` and write PNGs to `graphs/`. After the experiment CSVs exist:

```bash
gnuplot scripts/plot_fixed_n_time      # graphs/random_fixed_n.png
gnuplot scripts/plot_fixed_k_time      # graphs/random_fixed_k.png
gnuplot scripts/plot_fixed_n_mem       # graphs/random_fixed_n_mem.png
gnuplot scripts/plot_fixed_k_mem       # graphs/random_fixed_k_mem.png
gnuplot scripts/plot_read_random_M     # graphs/read_random_M.png
# ... and the plot_ehog_*, plot_aho_*, plot_wrt_ehog_* variants
```

Each script hard-codes its input CSV, output PNG, and column indices; open the one you want to confirm it matches your CSV column layout before running.

---

## The `apsphog` sub-project

`apsphog/` is an independent CMake project building a shared library for the all-pairs suffix–prefix HOG variant:

```bash
cd apsphog
cmake -B build && cmake --build build
```

---

## Typical end-to-end run

```bash
# 0. one-time setup
git clone https://github.com/shahbazk/HoG.git && cd HoG
#    place malloc_count/ at repo root; place datasets in data/

# 1. build construction benchmarks
bash scripts/build.sh

# 2. synthetic construction sweeps → plots/
bash scripts/test_random.sh
bash scripts/test_read_random.sh

# 3. real-data construction runs → dump/   (edit dataset list first)
bash scripts/script_combined.sh

# 4. query benchmark → LaTeX tables
cd tests && g++ -O3 -std=c++20 -I../include testQueriesKMP.cpp -o testQueriesKMP && ./testQueriesKMP > ../queries_tables.tex && cd ..

# 5. figures
gnuplot scripts/plot_fixed_n_time
gnuplot scripts/plot_fixed_k_time
gnuplot scripts/plot_read_random_M
```

---

## Notes & troubleshooting

- **`couldn't open file: <name>`** — the dataset isn't where the driver expects it: `data/` for the construction drivers, `../data/` for the query drivers (run those from `tests/`).
- **Link error on `malloc_count`** — the `malloc_count/` directory is missing; see Prerequisites.
- **`/usr/bin/time: No such file`** — install the standalone `time` package; the shell builtin won't accept the `-f`/`--output` flags the scripts rely on.
- **Everything reports `timeout`** — the slower baselines exceed the 30 s per-run cap on large inputs; raise the `timeout` value in the relevant `scripts/test_*.sh` if you need those cells.
- **Stage-order errors** — run `AHO` (and `EHOG` for `*EHOG` binaries) before a HOG variant on the same input, since they exchange intermediate files through `dump/`.
