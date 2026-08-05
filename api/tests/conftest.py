import pytest_asyncio


@pytest_asyncio.fixture
async def client():
    from httpx import AsyncClient, ASGITransport
    from main import app
    async with AsyncClient(
        transport=ASGITransport(app=app), base_url="http://test"
    ) as ac:
        yield ac
