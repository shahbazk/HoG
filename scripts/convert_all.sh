#!/bin/bash
# convert_all.sh — batch-convert raw FASTA downloads into HoG dataset files.
#
# Reads FASTA from a raw directory (default: ./raw) and writes datasets in the
# <k>/one-string-per-line, abcd-alphabet format into ./data, using the canonical
# dataset names the drivers and scripts expect.
#
# Each entry below maps an output dataset name to one or more raw input files
# (space-separated; multiple files are merged into one dataset). Edit the RHS to
# match the filenames you actually downloaded from:
#   plant EST : http://www.citrusgenomedb.org/ , http://www.uni-ulm.de/in/theo/research/seqana
#   bacterial : http://ccb.jhu.edu/gage_b/datasets/index.html
#
# A mapping whose raw file is missing is skipped with a message, so you can
# convert whatever subset you have.
#
# Usage:
#   bash scripts/convert_all.sh                 # raw/ -> data/
#   RAW_DIR=/path/to/raw bash scripts/convert_all.sh
#   bash scripts/convert_all.sh --keep-ambiguous   # pass-through to converter

set -u

RAW_DIR="${RAW_DIR:-raw}"
OUT_DIR="${OUT_DIR:-data}"
PY="python3 scripts/fasta_to_dataset.py"
EXTRA_ARGS="$*"   # e.g. --keep-ambiguous, --min-len 100

mkdir -p "$OUT_DIR"

# dataset_name : raw file(s) under $RAW_DIR (space-separated to merge)
# Adjust the raw filenames on the right to match your downloads.
declare -A MAP=(
    [clementina]="clementina.fasta"
    [sinensis]="sinensis.fasta"
    [trifoliata]="trifoliata.fasta"
    [elegans]="elegans.fasta"
    [A_hydrophila_HiSeq]="A_hydrophila_HiSeq.fasta"
    [B_cereus_HiSeq]="B_cereus_HiSeq.fasta"
    [B_cereus_MiSeq]="B_cereus_MiSeq.fasta"
    [B_fragilis_HiSeq]="B_fragilis_HiSeq.fasta"
    [M_abscessus_HiSeq]="M_abscessus_HiSeq.fasta"
    [M_abscessus_MiSeq]="M_abscessus_MiSeq.fasta"
    [R_sphaeroides_HiSeq]="R_sphaeroides_HiSeq.fasta"
    [R_sphaeroides_MiSeq]="R_sphaeroides_MiSeq.fasta"
    [S_aureus_HiSeq]="S_aureus_HiSeq.fasta"
    [V_cholerae_HiSeq]="V_cholerae_HiSeq.fasta"
    [V_cholerae_MiSeq]="V_cholerae_MiSeq.fasta"
    [X_axonopodis_HiSeq]="X_axonopodis_HiSeq.fasta"
)

converted=0
skipped=0
for name in "${!MAP[@]}"; do
    # resolve raw inputs (may be several, space-separated)
    inputs=()
    missing=0
    for f in ${MAP[$name]}; do
        if [ -f "$RAW_DIR/$f" ]; then
            inputs+=("$RAW_DIR/$f")
        else
            missing=1
        fi
    done

    if [ "$missing" -eq 1 ] || [ "${#inputs[@]}" -eq 0 ]; then
        echo "SKIP $name: raw file(s) not found in $RAW_DIR/ (${MAP[$name]})"
        skipped=$((skipped+1))
        continue
    fi

    echo "convert $name  <-  ${inputs[*]}"
    $PY "${inputs[@]}" -o "$OUT_DIR/$name" $EXTRA_ARGS
    converted=$((converted+1))
done

echo ""
echo "done: $converted converted, $skipped skipped -> $OUT_DIR/"
