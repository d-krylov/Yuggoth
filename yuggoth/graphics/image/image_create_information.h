#ifndef YUGGOTH_IMAGE_CREATE_INFORMATION_H
#define YUGGOTH_IMAGE_CREATE_INFORMATION_H

#include "walle/walle.h"

namespace Yuggoth {

struct ImageCreateInformation {

  static ImageCreateInformation CreateRenderTarget(uint32_t width, uint32_t height, Walle::Format format);
  static ImageCreateInformation CreateTexture2D(uint32_t width, uint32_t height, Walle::Format format, uint32_t levels);

  uint32_t levels_{1};
  uint32_t layers_{1};
  Walle::Format format_ = Walle::Format::E_UNDEFINED;
  Walle::Extent3D extent_{0, 0, 0};
  Walle::SampleCountMaskBits samples_ = Walle::SampleCountMaskBits::E_1_BIT;
  Walle::ImageUsageMask usage_;
  Walle::ImageType image_type_;
  Walle::ImageViewType image_view_type_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_CREATE_INFORMATION_H