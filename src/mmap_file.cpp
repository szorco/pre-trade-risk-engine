#include "lob/mmap_file.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <utility>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace lob {

static std::runtime_error sys_error(const char* what) {
  return std::runtime_error(std::string(what) + ": " + std::strerror(errno));
}

MMapFile::MMapFile(const std::string& path) {
  fd_ = ::open(path.c_str(), O_RDONLY);
  if (fd_ < 0) {
    throw sys_error("open");
  }

  struct stat st {};
  if (::fstat(fd_, &st) != 0) {
    const int saved = errno;
    ::close(fd_);
    fd_ = -1;
    errno = saved;
    throw sys_error("fstat");
  }

  if (st.st_size <= 0) {
    ::close(fd_);
    fd_ = -1;
    return;
  }

  size_ = static_cast<std::size_t>(st.st_size);
  void* mapped = ::mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0);
  if (mapped == MAP_FAILED) {
    const int saved = errno;
    ::close(fd_);
    fd_ = -1;
    size_ = 0;
    errno = saved;
    throw sys_error("mmap");
  }

  data_ = static_cast<const std::byte*>(mapped);
}

MMapFile::MMapFile(MMapFile&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      fd_(std::exchange(other.fd_, -1)) {}

MMapFile& MMapFile::operator=(MMapFile&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  close();
  data_ = std::exchange(other.data_, nullptr);
  size_ = std::exchange(other.size_, 0);
  fd_ = std::exchange(other.fd_, -1);
  return *this;
}

MMapFile::~MMapFile() { close(); }

void MMapFile::close() noexcept {
  if (data_ != nullptr && size_ != 0) {
    ::munmap(const_cast<std::byte*>(data_), size_);
  }
  if (fd_ >= 0) {
    ::close(fd_);
  }
  data_ = nullptr;
  size_ = 0;
  fd_ = -1;
}

}  // namespace lob

