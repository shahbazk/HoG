#!/bin/bash
# Real-dataset construction benchmark.
# Requires datasets in ./data/<name> (see README for the file format).
# Uses the *_REAL binaries (input mode INPUT_REAL: argv[1] = dataset name).

set -u

# --- build (out-of-source; matches scripts/build.sh) ---
cmake -B build
cmake --build build -j"$(nproc)" \
    --target AHO_REAL EHOG_REAL \
             SSP_REAL SK_REAL EC_REAL BCER_REAL \
             SSPEHOG_REAL ECEHOG_REAL

# --- output dirs (dump/{data,aho,ehog} are created by AHO_REAL itself) ---
mkdir -p dump
# fresh output/mem files (|| true so a missing file is not an error)
for f in ssp sk ec bcer ehog ehogx; do
    rm -f "./dump/output${f}" "./dump/memprof${f}"
done

format="%M,%e,%U,%S"

s=(
    clementina
    sinensis
    trifoliata
    elegans
    A_hydrophila_HiSeq
    B_cereus_HiSeq
    B_cereus_MiSeq
    B_fragilis_HiSeq
    M_abscessus_HiSeq
    M_abscessus_MiSeq
    R_sphaeroides_HiSeq
    R_sphaeroides_MiSeq
    S_aureus_HiSeq
    V_cholerae_HiSeq
    V_cholerae_MiSeq
    X_axonopodis_HiSeq
)

for dataset in "${s[@]}"; do
    # skip datasets that are not present, with a clear message
    if [ ! -f "./data/${dataset}" ]; then
        echo "SKIP ${dataset}: ./data/${dataset} not found"
        continue
    fi

    # 1. build shared intermediates (Aho-Corasick, then EHOG) for this dataset
    ./bin/AHO_REAL  "$dataset" > /dev/null
    ./bin/EHOG_REAL "$dataset" > /dev/null

    # 2. via-EHOG stages
    /usr/bin/time -f "$format" --output=./dump/memprofehog  -a ./bin/SSPEHOG_REAL "$dataset" >> ./dump/outputehog
    /usr/bin/time -f "$format" --output=./dump/memprofehogx -a ./bin/ECEHOG_REAL  "$dataset" >> ./dump/outputehogx
    echo "$dataset done ehog, ehogx"

    # 3. direct stages
    /usr/bin/time -f "$format" --output=./dump/memprofssp -a ./bin/SSP_REAL "$dataset" >> ./dump/outputssp
    /usr/bin/time -f "$format" --output=./dump/memprofsk  -a ./bin/SK_REAL  "$dataset" >> ./dump/outputsk
    /usr/bin/time -f "$format" --output=./dump/memprofec  -a ./bin/EC_REAL  "$dataset" >> ./dump/outputec
    echo "$dataset done for ssp, sk, ec, ehog, ehogx"
done

# BCER only on the smaller datasets (as in the original script)
s2=(
    clementina
    sinensis
    trifoliata
    elegans
    B_cereus_MiSeq
    M_abscessus_MiSeq
)
for dataset in "${s2[@]}"; do
    if [ ! -f "./data/${dataset}" ]; then
        echo "SKIP ${dataset} (BCER): ./data/${dataset} not found"
        continue
    fi
    # BCER needs the AHO intermediate; ensure it exists (cheap if already built)
    ./bin/AHO_REAL "$dataset" > /dev/null
    /usr/bin/time -f "$format" --output=./dump/memprofbcer -a ./bin/BCER_REAL "$dataset" >> ./dump/outputbcer
    echo "$dataset done for bcer"
done
