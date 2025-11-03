#ifndef YUGGOTH_IMAGE_2D_H
#define YUGGOTH_IMAGE_2D_H

#include "image.h"
#include <span>

namespace Yuggoth {

class Image2D {
public:
  Image2D(uint32_t width, uint32_t height, std::span<const std::byte> data);

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_2D_H