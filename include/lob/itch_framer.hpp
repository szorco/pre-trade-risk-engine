#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include "lob/endian.hpp"

namespace lob {

struct ItchRecordView {
  char type = '\0';
  const std::byte* payload = nullptr;  // points to message type (1 byte) then body
  std::uint16_t payload_len = 0;       // bytes following the 2-byte length field
};

class ItchFramer {
 public:
  ItchFramer(const std::byte* data, std::size_t size) : data_(data), size_(size) {}

  std::optional<ItchRecordView> next() noexcept {
    if (offset_ + 2 > size_) {
      return std::nullopt;
    }

    const auto payload_len = read_be_u16(data_ + offset_);
    const std::size_t record_total = 2ULL + static_cast<std::size_t>(payload_len);

    if (payload_len < 1) {
      // Must contain at least a 1-byte message type.
      return std::nullopt;
    }
    if (offset_ + record_total > size_) {
      return std::nullopt;
    }

    const std::byte* payload = data_ + offset_ + 2;
    const char type = static_cast<char>(payload[0]);

    ItchRecordView view;
    view.type = type;
    view.payload = payload;
    view.payload_len = payload_len;

    offset_ += record_total;
    return view;
  }

  std::size_t offset() const noexcept { return offset_; }

 private:
  const std::byte* data_ = nullptr;
  std::size_t size_ = 0;
  std::size_t offset_ = 0;
};

}  // namespace lob

