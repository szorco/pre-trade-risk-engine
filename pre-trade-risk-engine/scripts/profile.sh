#!/usr/bin/env bash
# scripts/profile.sh — Linux perf profiling helper
# Usage: ./scripts/profile.sh [output_dir]
set -euo pipefail

BINARY="./build/risk_engine_bin"
OUTPUT_DIR="${1:-perf_output}"
PERF_DATA="$OUTPUT_DIR/perf.data"

if [[ ! -f "$BINARY" ]]; then
    echo "[ERROR] Binary not found at $BINARY — run cmake build first."
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

echo "[INFO] Recording with perf (call-graph dwarf)..."
perf record \
    --call-graph dwarf \
    -e cycles,cache-misses,cache-references,branch-misses \
    -o "$PERF_DATA" \
    -- "$BINARY"

echo "[INFO] Generating report..."
perf report \
    --input="$PERF_DATA" \
    --stdio \
    > "$OUTPUT_DIR/perf_report.txt"

echo "[INFO] Generating flame graph data..."
perf script --input="$PERF_DATA" > "$OUTPUT_DIR/perf.script"

echo ""
echo "Artifacts written to $OUTPUT_DIR/"
echo "  perf_report.txt  — annotated report"
echo "  perf.script      — flame graph input (pipe to stackcollapse-perf.pl + flamegraph.pl)"
echo ""
echo "TODO: pipe perf.script through FlameGraph tools for SVG output"
echo "      https://github.com/brendangregg/FlameGraph"
