#ifndef YUGGOTH_CORE_TYPES_H
#define YUGGOTH_CORE_TYPES_H

#include <cstdint>

namespace Yuggoth {

struct ViewportSize {
  bool operator==(const ViewportSize &other) const = default;
  int32_t width;
  int32_t height;
};

struct CursorPosition {
  double x;
  double y;
};

struct BufferRange {
  uint32_t stride_ = 0; // element size
  uint32_t offset_ = 0; // base element index
  uint32_t count_ = 0;  // number of elements
};

} // namespace Yuggoth

#endif // YUGGOTH_CORE_TYPES_H
