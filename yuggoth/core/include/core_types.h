#ifndef YUGGOTH_CORE_TYPES_H
#define YUGGOTH_CORE_TYPES_H

#include <cstdint>

namespace Yuggoth {

struct ViewportSize {

  auto GetAspect() const {
    return static_cast<float>(width) / static_cast<float>(height);
  }

  int32_t width;
  int32_t height;
};

} // namespace Yuggoth

#endif // YUGGOTH_CORE_TYPES_H
