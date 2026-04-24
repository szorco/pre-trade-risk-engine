#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "lob/itch_framer.hpp"
#include "lob/mmap_file.hpp"

namespace {

void print_usage(const char* argv0) {
  std::cerr << "Usage:\n";
  std::cerr << "  " << argv0 << " count <path-to-itch-file>\n";
}

int cmd_count(const std::string& path) {
  lob::MMapFile file(path);
  if (!file) {
    std::cerr << "Failed to mmap: " << path << "\n";
    return 2;
  }

  std::array<std::uint64_t, 256> counts{};

  lob::ItchFramer framer(file.data(), file.size());
  while (auto rec = framer.next()) {
    const unsigned char idx = static_cast<unsigned char>(rec->type);
    counts[idx] += 1;
  }

  std::cout << "Message counts:\n";
  for (std::size_t i = 0; i < counts.size(); ++i) {
    if (counts[i] == 0) {
      continue;
    }
    const char c = static_cast<char>(i);
    if (std::isprint(static_cast<unsigned char>(c)) != 0) {
      std::cout << "  '" << c << "': " << counts[i] << "\n";
    } else {
      std::cout << "  0x" << std::hex << std::setw(2) << std::setfill('0') << i << std::dec
                << ": " << counts[i] << "\n";
    }
  }
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 2;
  }

  const std::string cmd = argv[1];
  if (cmd == "-h" || cmd == "--help" || cmd == "help") {
    print_usage(argv[0]);
    return 0;
  }
  if (cmd == "count") {
    if (argc != 3) {
      print_usage(argv[0]);
      return 2;
    }
    return cmd_count(argv[2]);
  }

  print_usage(argv[0]);
  return 2;
}
