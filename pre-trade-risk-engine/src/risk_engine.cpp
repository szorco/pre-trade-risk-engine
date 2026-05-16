#include "risk_engine/risk_engine.hpp"

namespace risk_engine {

RiskEngine::RiskEngine(double max_position, double max_exposure)
    : m_max_position(max_position)
    , m_max_exposure(max_exposure)
{
    // TODO: any one-time initialization (e.g., pre-warming caches)
}

RiskEngine::~RiskEngine() {
    // TODO: graceful shutdown if background threads are added
}

RiskResult RiskEngine::validate(const Order& order) {
    auto& stripe = m_stripes[stripe_index(order.instrument_id)];
    std::lock_guard<std::mutex> lock(stripe.mtx);

    // TODO: implement position limit check
    //   e.g., if (stripe.net_position + delta > m_max_position) return REJECTED_POSITION_LIMIT;

    // TODO: implement exposure limit check
    //   e.g., if (stripe.gross_exposure + notional > m_max_exposure) return REJECTED_EXPOSURE_LIMIT;

    // TODO: update stripe state on approval
    //   stripe.net_position  += ...;
    //   stripe.gross_exposure += ...;

    return RiskResult::APPROVED;
}

}  // namespace risk_engine
