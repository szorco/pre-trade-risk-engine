#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace lob {

class MMapFile {
 public:
  MMapFile() = default;
  explicit MMapFile(const std::string& path);

  MMapFile(const MMapFile&) = delete;
  MMapFile& operator=(const MMapFile&) = delete;

  MMapFile(MMapFile&& other) noexcept;
  MMapFile& operator=(MMapFile&& other) noexcept;

  ~MMapFile();

  const std::byte* data() const noexcept { return data_; }
  std::size_t size() const noexcept { return size_; }
  explicit operator bool() const noexcept { return data_ != nullptr; }

  void close() noexcept;

 private:
  const std::byte* data_ = nullptr;
  std::size_t size_ = 0;
  int fd_ = -1;
};

}  // namespace lob

