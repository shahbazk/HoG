#!/usr/bin/env python3
"""
fasta_to_dataset.py — convert raw (multi-)FASTA into the HoG dataset format.

The HoG drivers (construction + query) read a dataset file as:

    <k>
    <string_1>
    <string_2>
    ...
    <string_k>

i.e. a count on the first line, then one string per line. The trie uses
c = ch - 'a' with alphabet = 4, so strings must be over the 4-letter alphabet
{a, b, c, d}. Genomic sequences are A/C/G/T, so this script both reshapes the
FASTA and remaps the nucleotides:

    A -> a,  C -> b,  G -> c,  T -> d   (case-insensitive)

Each FASTA record becomes one string (its sequence lines concatenated, headers
dropped). Records containing characters outside ACGT (e.g. N or IUPAC
ambiguity codes) are, by default, skipped; use --keep-ambiguous to instead drop
just the offending characters and keep the record.

Usage:
    python3 scripts/fasta_to_dataset.py INPUT.fasta -o data/<name>
    python3 scripts/fasta_to_dataset.py INPUT.fasta                 # -> stdout
    python3 scripts/fasta_to_dataset.py *.fa -o data/<name>         # merge many

Options:
    -o, --output PATH     write here (default: stdout)
    --min-len N           drop records shorter than N (default: 1)
    --keep-ambiguous      drop non-ACGT characters instead of skipping the record
"""

import argparse
import sys

MAP = str.maketrans({
    "A": "a", "a": "a",
    "C": "b", "c": "b",
    "G": "c", "g": "c",
    "T": "d", "t": "d",
})
VALID = set("ACGTacgt")


def iter_fasta(paths):
    """Yield full sequence strings (one per record) across all input files."""
    header = None
    seq_parts = []
    for path in paths:
        fh = sys.stdin if path == "-" else open(path, "r")
        try:
            for line in fh:
                line = line.rstrip("\n\r")
                if not line:
                    continue
                if line[0] == ">":
                    if header is not None:
                        yield "".join(seq_parts)
                    header = line[1:]
                    seq_parts = []
                else:
                    seq_parts.append(line)
        finally:
            if fh is not sys.stdin:
                fh.close()
    if header is not None:
        yield "".join(seq_parts)


def convert(seq, keep_ambiguous):
    """Return the remapped a/b/c/d string, or None if the record is rejected."""
    if keep_ambiguous:
        seq = "".join(ch for ch in seq if ch in VALID)
    else:
        if any(ch not in VALID for ch in seq):
            return None
    return seq.translate(MAP)


def main():
    ap = argparse.ArgumentParser(description="Convert FASTA to HoG dataset format.")
    ap.add_argument("inputs", nargs="+", help="FASTA file(s); '-' for stdin")
    ap.add_argument("-o", "--output", default="-", help="output path (default stdout)")
    ap.add_argument("--min-len", type=int, default=1, help="drop records shorter than N")
    ap.add_argument("--keep-ambiguous", action="store_true",
                    help="drop non-ACGT chars instead of skipping the record")
    args = ap.parse_args()

    strings = []
    skipped = 0
    for raw in iter_fasta(args.inputs):
        s = convert(raw, args.keep_ambiguous)
        if s is None or len(s) < args.min_len:
            skipped += 1
            continue
        strings.append(s)

    out = sys.stdout if args.output == "-" else open(args.output, "w")
    try:
        out.write(f"{len(strings)}\n")
        for s in strings:
            out.write(s + "\n")
    finally:
        if out is not sys.stdout:
            out.close()

    if skipped:
        sys.stderr.write(f"note: skipped {skipped} record(s) "
                         f"(too short or non-ACGT; see --keep-ambiguous)\n")
    sys.stderr.write(f"wrote {len(strings)} strings"
                     f"{'' if args.output == '-' else ' to ' + args.output}\n")


if __name__ == "__main__":
    main()
