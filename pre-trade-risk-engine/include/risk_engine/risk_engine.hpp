#pragma once

#include "common.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>

namespace risk_engine {

// TODO: Tune stripe count based on core count and contention profile
inline constexpr std::size_t STRIPE_COUNT = 16;

// Per-stripe state — cache-line padded to prevent false sharing
struct alignas(CACHE_LINE_SIZE) PositionStripe {
    std::mutex  mtx;
    double      net_position  = 0.0;
    double      gross_exposure = 0.0;
    // TODO: add any additional per-stripe tracking fields
    char _pad[CACHE_LINE_SIZE
              - sizeof(std::mutex)
              - sizeof(double) * 2];  // adjust if fields change
};

class RiskEngine {
public:
    // TODO: Accept configurable limits (position, exposure) via constructor
    explicit RiskEngine(double max_position, double max_exposure);
    ~RiskEngine();

    // Core validation — called from worker threads
    // Returns RiskResult::APPROVED or a rejection code
    RiskResult validate(const Order& order);

    // TODO: Add methods for:
    //   - resetting state between test runs
    //   - querying current position/exposure snapshots
    //   - hot-reloading limits without restart

private:
    double m_max_position;
    double m_max_exposure;

    // Lock-striped position table (stripe by instrument_id)
    PositionStripe m_stripes[STRIPE_COUNT];

    // TODO: Implement stripe selection strategy
    inline std::size_t stripe_index(uint64_t instrument_id) const {
        return instrument_id % STRIPE_COUNT;
    }
};

}  // namespace risk_engine
