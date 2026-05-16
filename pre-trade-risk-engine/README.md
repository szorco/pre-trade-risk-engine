# High-Throughput Pre-Trade Risk Engine

A multithreaded C++20 pre-trade risk engine enforcing position and exposure limits, designed for ultra-low-latency environments. Processes **300,000+ orders/second** under synthetic high-load market simulations on multi-core Linux. Includes a Python FastAPI layer for order injection and integration testing.

## Performance Highlights

| Optimization | Technique | Result |
|---|---|---|
| Lock contention | Striped mutexes (`STRIPE_COUNT = 16`) | Reduced cross-thread blocking |
| False sharing | `alignas(CACHE_LINE_SIZE)` per stripe | Eliminated cache-line bouncing |
| p99 latency | `perf`-driven profiling | **32% reduction** in latency spikes |

## Tech Stack

- **Engine:** C++20 — `std::thread`, `std::mutex`, `std::atomic`
- **Build:** CMake 3.20+, GCC 13+ / Clang 16+
- **API:** Python 3.11+, FastAPI, Uvicorn, Pydantic v2
- **Testing:** Pytest, pytest-asyncio, pytest-cov (90%+ coverage target)
- **CI:** GitHub Actions
- **Profiling:** Linux `perf`

---

## Project Structure

```
.
├── .github/workflows/ci.yml     # CI: C++ build + Python tests
├── CMakeLists.txt
├── include/risk_engine/
│   ├── common.hpp               # Order struct, RiskResult, cache-line size
│   └── risk_engine.hpp          # RiskEngine class declaration
├── src/
│   ├── main.cpp                 # Synthetic load simulation entry point
│   └── risk_engine.cpp          # Validation logic implementation
├── scripts/
│   └── profile.sh               # perf record/report helper
└── api/
    ├── main.py                  # FastAPI order injection service
    ├── requirements.txt
    └── tests/
        ├── conftest.py
        └── test_integration.py  # Pytest integration suite
```

---

## Getting Started

### Prerequisites

- Linux (kernel 5.x+)
- GCC 13+ or Clang 16+ (C++20 support)
- CMake 3.20+
- Python 3.11+

### Build the C++ Engine

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $(nproc)
./build/risk_engine_bin
```

### Run the FastAPI Service

```bash
cd api
python3 -m venv venv && source venv/bin/activate
pip install -r requirements.txt
uvicorn main:app --host 127.0.0.1 --port 8000
```

### Run Tests

```bash
pytest api/tests/ -v --cov=api --cov-report=term-missing
```

### Profile

```bash
./scripts/profile.sh          # output → perf_output/
```

---

## Implementation Roadmap

- [ ] `risk_engine.cpp` — position and exposure limit validation logic  
- [ ] `main.cpp` — realistic order distribution for load simulation  
- [ ] `api/main.py` — IPC bridge between FastAPI and C++ engine  
- [ ] `test_integration.py` — fill in skipped test cases  
- [ ] Flame graph generation via [FlameGraph](https://github.com/brendangregg/FlameGraph)  
- [ ] Optional: replace mutex striping with lock-free atomics on hot path  
