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

} // namespace Yuggoth

#endif // YUGGOTH_CORE_TYPES_H
