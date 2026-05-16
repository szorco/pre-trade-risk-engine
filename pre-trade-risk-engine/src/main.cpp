#include "risk_engine/risk_engine.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// TODO: Make these configurable via CLI args or config file
static constexpr double MAX_POSITION   = 1'000'000.0;
static constexpr double MAX_EXPOSURE   = 50'000'000.0;
static constexpr int    NUM_THREADS    = 8;
static constexpr int    ORDERS_PER_THREAD = 100'000;

int main() {
    using namespace risk_engine;

    RiskEngine engine(MAX_POSITION, MAX_EXPOSURE);

    std::atomic<uint64_t> approved{0};
    std::atomic<uint64_t> rejected{0};

    // TODO: Replace synthetic order generation with real feed/injection
    auto worker = [&](int thread_id) {
        for (int i = 0; i < ORDERS_PER_THREAD; ++i) {
            Order order{};
            order.order_id      = static_cast<uint64_t>(thread_id) * ORDERS_PER_THREAD + i;
            order.instrument_id = i % 64;   // TODO: vary instrument distribution
            order.quantity      = 100.0;    // TODO: vary size distribution
            order.price         = 50.0;     // TODO: vary price
            order.side          = (i % 2 == 0) ? 1 : -1;

            auto result = engine.validate(order);
            if (result == RiskResult::APPROVED) ++approved;
            else                                ++rejected;
        }
    };

    auto t_start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);
    for (int t = 0; t < NUM_THREADS; ++t)
        threads.emplace_back(worker, t);
    for (auto& th : threads)
        th.join();

    auto t_end   = std::chrono::steady_clock::now();
    double elapsed_s = std::chrono::duration<double>(t_end - t_start).count();
    uint64_t total   = approved + rejected;

    std::cout << "Orders processed : " << total      << "\n"
              << "Approved         : " << approved   << "\n"
              << "Rejected         : " << rejected   << "\n"
              << "Elapsed (s)      : " << elapsed_s  << "\n"
              << "Throughput (ord/s): " << static_cast<uint64_t>(total / elapsed_s) << "\n";

    return 0;
}
