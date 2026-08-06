# Pre-Trade Risk Engine

A pre-trade risk engine that validates orders against configurable limits before they reach the market. 

## What's here

- `CMakeLists.txt` — builds C++20 binary (`risk_engine_bin`).
- `src/main.cpp` — placeholder entry point that compiles and runs.
- `include/risk_engine/` — headers
- `api/` — FastAPI service with a `/health` route.
- `api/tests/` — health check.
- `scripts/profile.sh` — Linux `perf` profiling helper.
- `.github/workflows/ci.yml` — CI: builds the C++ binary, runs the Python tests.

## Build & run (C++)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(nproc)"
./build/risk_engine_bin
```

## Run the API

```bash
cd api
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
uvicorn main:app --reload
```

## Run tests

```bash
cd api
pytest tests/ -v --cov=. --cov-report=term-missing
```

## Profile

```bash
./scripts/profile.sh   # output -> perf_output/
```


## How does it work (high-level overview)

Given some Trade input, the engine asks if this trade fits within in our criteria of risk, limits, and other constraints. The current criteria is as follows (more to come later):
- **Net Postion Limit** : (total shares in one instrument)
- **Gross Exposure Limit** : (total risk in market regardless of direction)

