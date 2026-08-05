#!/usr/bin/env bash
# Linux perf profiling helper.  Usage: ./scripts/profile.sh [output_dir]
set -euo pipefail

BINARY="./build/risk_engine_bin"
OUTPUT_DIR="${1:-perf_output}"
PERF_DATA="$OUTPUT_DIR/perf.data"

if [[ ! -f "$BINARY" ]]; then
    echo "[ERROR] Binary not found at $BINARY — build first (see README)."
    exit 1
fi

mkdir -p "$OUTPUT_DIR"
echo "[INFO] Recording with perf..."
perf record --call-graph dwarf -o "$PERF_DATA" -- "$BINARY"
echo "[INFO] Writing report..."
perf report --input="$PERF_DATA" --stdio > "$OUTPUT_DIR/perf_report.txt"
echo "Done -> $OUTPUT_DIR/"
