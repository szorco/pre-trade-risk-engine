#pragma once

#include <cstddef>
#include <cstdint>

namespace lob {

inline std::uint16_t read_be_u16(const std::byte* p) noexcept {
  return (static_cast<std::uint16_t>(static_cast<unsigned char>(p[0])) << 8) |
         (static_cast<std::uint16_t>(static_cast<unsigned char>(p[1])) << 0);
}

inline std::uint32_t read_be_u32(const std::byte* p) noexcept {
  return (static_cast<std::uint32_t>(static_cast<unsigned char>(p[0])) << 24) |
         (static_cast<std::uint32_t>(static_cast<unsigned char>(p[1])) << 16) |
         (static_cast<std::uint32_t>(static_cast<unsigned char>(p[2])) << 8) |
         (static_cast<std::uint32_t>(static_cast<unsigned char>(p[3])) << 0);
}

inline std::uint64_t read_be_u64(const std::byte* p) noexcept {
  return (static_cast<std::uint64_t>(static_cast<unsigned char>(p[0])) << 56) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[1])) << 48) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[2])) << 40) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[3])) << 32) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[4])) << 24) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[5])) << 16) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[6])) << 8) |
         (static_cast<std::uint64_t>(static_cast<unsigned char>(p[7])) << 0);
}

}  // namespace lob

