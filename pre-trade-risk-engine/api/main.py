from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import httpx

app = FastAPI(title="Pre-Trade Risk Engine — Order Injection API")


# ---------------------------------------------------------------------------
# Request / Response schemas
# ---------------------------------------------------------------------------

class OrderRequest(BaseModel):
    order_id: int
    instrument_id: int
    quantity: float
    price: float
    side: int  # 1 = buy, -1 = sell
    # TODO: add additional fields matching your C++ Order struct


class OrderResponse(BaseModel):
    order_id: int
    result: str          # "APPROVED" | "REJECTED_*"
    reason: str | None = None
    # TODO: add latency_us, timestamp, etc. for observability


# ---------------------------------------------------------------------------
# Routes
# ---------------------------------------------------------------------------

@app.get("/health")
async def health() -> dict:
    return {"status": "ok"}


@app.post("/orders", response_model=OrderResponse)
async def submit_order(order: OrderRequest) -> OrderResponse:
    """
    Inject a single order into the risk engine.
    TODO: forward this over a socket / shared-memory queue to the C++ engine
          and return the actual RiskResult.
    """
    # Placeholder — replace with real IPC call to the C++ binary
    raise HTTPException(status_code=501, detail="Engine IPC not yet implemented")


@app.post("/orders/batch")
async def submit_batch(orders: list[OrderRequest]) -> list[OrderResponse]:
    """
    Inject a batch of orders.
    TODO: implement batch submission for throughput testing.
    """
    raise HTTPException(status_code=501, detail="Batch submission not yet implemented")


# TODO: Add endpoints for:
#   POST /config/limits  — hot-reload position/exposure limits
#   GET  /metrics        — current position snapshots per instrument
#   POST /reset          — reset engine state between test runs
