from fastapi import FastAPI

app = FastAPI(title="Pre-Trade Risk Engine API")


@app.get("/health")
async def health() -> dict:
    return {"status": "ok"}

# Add your order-injection routes here as you design the API.
