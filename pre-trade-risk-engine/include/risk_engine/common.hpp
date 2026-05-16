#pragma once

#include <cstdint>
#include <new>  // std::hardware_destructive_interference_size

namespace risk_engine {

// Cache-line size for alignment (typically 64 bytes on x86-64)
#ifdef __cpp_lib_hardware_interference_size
    inline constexpr std::size_t CACHE_LINE_SIZE =
        std::hardware_destructive_interference_size;
#else
    inline constexpr std::size_t CACHE_LINE_SIZE = 64;
#endif

// TODO: Define your order representation
struct Order {
    uint64_t order_id;
    uint64_t instrument_id;
    double   quantity;
    double   price;
    int      side;   // e.g., 1 = buy, -1 = sell
    // TODO: add additional fields as needed
};

// TODO: Define risk check result codes
enum class RiskResult : uint8_t {
    APPROVED = 0,
    REJECTED_POSITION_LIMIT,
    REJECTED_EXPOSURE_LIMIT,
    // TODO: add more rejection reasons
};

}  // namespace risk_engine
