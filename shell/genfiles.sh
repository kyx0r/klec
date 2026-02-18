#!/bin/sh
# Generate two 15 MB files in the directory specified by $1
# File 1: deterministic (fixed seed) random data — same bytes on any system
# File 2: purely random data

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <output-directory>" >&2
    exit 1
fi

OUTDIR="$1"
SIZE=$((15 * 1024 * 1024))  # 15 MB in bytes
SEED="deadbeef"

mkdir -p "$OUTDIR"

# Fixed-seed file: use AES-256-CTR in a known key+IV derived from the seed.
# openssl enc with a fixed password and fixed salt produces identical output
# on every system regardless of platform.
echo "Generating fixed-seed file: $OUTDIR/fixed_seed.bin"
openssl enc -aes-256-ctr -pass pass:"$SEED" -nosalt -in /dev/zero 2>/dev/null \
    | head -c "$SIZE" > "$OUTDIR/fixed_seed.bin"

# Purely random file
echo "Generating random file: $OUTDIR/random.bin"
openssl rand "$SIZE" > "$OUTDIR/random.bin"

echo "Done."
ls -lh "$OUTDIR/fixed_seed.bin" "$OUTDIR/random.bin"
