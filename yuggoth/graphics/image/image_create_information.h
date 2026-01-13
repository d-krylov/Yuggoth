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

struct ImageViewCreateInformation {

  static ImageViewCreateInformation CreateDefault(Walle::ImageViewType image_view_type, Walle::Format format);

  Walle::Format format_ = Walle::Format::E_UNDEFINED;
  Walle::ImageViewType image_view_type_;
  Walle::ImageAspectMask aspect_;
  uint32_t base_level_ = 0;
  uint32_t base_layer_ = 0;
  uint32_t level_count_ = 1;
  uint32_t layer_count_ = 1;
};

struct SamplerCreateInformation {
  Walle::Filter min_filter_ = Walle::Filter::E_LINEAR;
  Walle::Filter mag_filter_ = Walle::Filter::E_LINEAR;
  Walle::SamplerMipmapMode mipmap_mode_ = Walle::SamplerMipmapMode::E_LINEAR;
  Walle::SamplerAddressMode address_mode_u_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
  Walle::SamplerAddressMode address_mode_v_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
  Walle::SamplerAddressMode address_mode_w_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_CREATE_INFORMATION_H