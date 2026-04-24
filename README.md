# Low-Latency Microstructure Signal Engine

High-performance C++20 engine for real-time limit order book (LOB) reconstruction and stochastic trade-flow analysis.

## Scope (phased)

1. **Data ingestion & framing**: memory-mapped ITCH files, record framing, message dispatch.
2. **Core LOB reconstruction**: correctness-first order book, deterministic replay.
3. **Hot path optimization**: slab allocator, thread pinning, lock-free SPSC handoff.
4. **Signal engine**: Hawkes process intensity + VPIN-style toxicity metrics.
5. **Hardware acceleration**: SIMD imbalance across depth, cache-line padding.
6. **Benchmarking**: microbenchmarks + latency profiles (P50/P99).

## Build (Linux / WSL)

If you don't have CMake installed in WSL yet:

```bash
sudo apt-get update
sudo apt-get install -y cmake ninja-build
```

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/lob_cli --help
```

Quick compile (no CMake):

```bash
mkdir -p build
g++ -std=c++20 -O2 -Iinclude src/main.cpp src/mmap_file.cpp -o build/lob_cli
```

## What exists today (base level)

- Memory-mapped file reader (`include/lob/mmap_file.hpp`)
- ITCH record framer / iterator (`include/lob/itch_framer.hpp`)
- Simple CLI that counts message types from an ITCH file (`src/main.cpp`)

## Next (slow and steady)

When you’re ready, we’ll implement the first *real* ITCH decoders (Add/Execute/Cancel/Delete) and a correctness-first book, then add snapshot checks.
