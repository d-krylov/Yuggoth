#ifndef YUGGOTH_IMAGE_DEPTH_H
#define YUGGOTH_IMAGE_DEPTH_H

#include "image.h"

namespace Yuggoth {

class ImageDepth : public Image {
public:
  ImageDepth() = default;

  ImageDepth(uint32_t width, uint32_t height, Format format = Format::E_D32_SFLOAT);

  ImageDepth(ImageDepth &&other) noexcept = default;

  ImageDepth &operator=(ImageDepth &&other) noexcept = default;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_DEPTH_H