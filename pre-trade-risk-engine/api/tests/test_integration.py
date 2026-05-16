"""
Integration tests for the Pre-Trade Risk Engine FastAPI service.
Run with:  pytest api/tests/ -v --cov=api --cov-report=term-missing
"""
import pytest
from httpx import AsyncClient, ASGITransport
from main import app


# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------

@pytest.fixture
def valid_order() -> dict:
    return {
        "order_id": 1,
        "instrument_id": 42,
        "quantity": 100.0,
        "price": 50.0,
        "side": 1,
    }


@pytest.fixture
def oversized_order() -> dict:
    """An order that should breach position limits."""
    return {
        "order_id": 2,
        "instrument_id": 42,
        "quantity": 999_999_999.0,  # TODO: align with configured max
        "price": 50.0,
        "side": 1,
    }


@pytest.fixture
async def client():
    async with AsyncClient(
        transport=ASGITransport(app=app), base_url="http://test"
    ) as ac:
        yield ac


# ---------------------------------------------------------------------------
# Health check
# ---------------------------------------------------------------------------

@pytest.mark.asyncio
async def test_health(client):
    resp = await client.get("/health")
    assert resp.status_code == 200
    assert resp.json()["status"] == "ok"


# ---------------------------------------------------------------------------
# Order validation — happy path
# ---------------------------------------------------------------------------

@pytest.mark.asyncio
async def test_valid_order_approved(client, valid_order):
    """A well-formed order within limits must be APPROVED."""
    # TODO: remove skip once engine IPC is implemented
    pytest.skip("Engine IPC not yet implemented")
    resp = await client.post("/orders", json=valid_order)
    assert resp.status_code == 200
    assert resp.json()["result"] == "APPROVED"


# ---------------------------------------------------------------------------
# Order validation — rejection paths
# ---------------------------------------------------------------------------

@pytest.mark.asyncio
async def test_position_limit_rejected(client, oversized_order):
    """An order breaching the position limit must be REJECTED."""
    pytest.skip("Engine IPC not yet implemented")
    resp = await client.post("/orders", json=oversized_order)
    assert resp.status_code == 200
    assert "REJECTED" in resp.json()["result"]


@pytest.mark.asyncio
async def test_exposure_limit_rejected(client):
    """TODO: construct an order that breaches exposure limit and assert rejection."""
    pytest.skip("TODO: implement exposure limit test")


# ---------------------------------------------------------------------------
# Batch submission
# ---------------------------------------------------------------------------

@pytest.mark.asyncio
async def test_batch_submission(client, valid_order):
    """TODO: test batch order injection endpoint."""
    pytest.skip("Batch submission not yet implemented")


# ---------------------------------------------------------------------------
# Concurrency / throughput smoke test
# ---------------------------------------------------------------------------

@pytest.mark.asyncio
async def test_concurrent_orders(client, valid_order):
    """
    TODO: fire N concurrent requests and assert all receive a valid response.
    Use asyncio.gather or pytest-asyncio to simulate concurrent load.
    """
    pytest.skip("TODO: implement concurrency smoke test")


# ---------------------------------------------------------------------------
# TODO: Add tests for —
#   - malformed / missing fields (422 responses)
#   - hot-reload of limits via POST /config/limits
#   - metrics endpoint returning per-instrument snapshots
#   - state reset between runs via POST /reset
# ---------------------------------------------------------------------------
