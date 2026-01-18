#ifndef YUGGOTH_TOOLS_H
#define YUGGOTH_TOOLS_H

#include <cstdint>
#include <utility>

namespace Yuggoth {

constexpr inline std::size_t operator""_KiB(unsigned long long int x) {
  return 1024ULL * x;
}

constexpr inline std::size_t operator""_MiB(unsigned long long int x) {
  return 1024_KiB * x;
}

constexpr inline std::size_t AlignUp(std::size_t value, std::size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}

template <typename T> inline constexpr std::size_t GetTypeId() {
  static int x;
  return reinterpret_cast<std::size_t>(&x);
}

using vector2i = std::pair<int32_t, int32_t>;
using vector2d = std::pair<double, double>;

struct ElementRange {
  uint32_t stride_ = 0; // element size
  uint32_t offset_ = 0; // base element index
  uint32_t count_ = 0;  // number of elements
};

} // namespace Yuggoth

#endif // YUGGOTH_TOOLS_H
