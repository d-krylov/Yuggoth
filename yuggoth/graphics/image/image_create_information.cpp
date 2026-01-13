#include "image_create_information.h"

namespace Yuggoth {

ImageCreateInformation ImageCreateInformation::CreateRenderTarget(uint32_t width, uint32_t height, Walle::Format format) {
  auto usage = Walle::IsFormatDepthOrStencil(format) ? Walle::ImageUsageMaskBits::E_DEPTH_STENCIL_ATTACHMENT_BIT //
                                                     : Walle::ImageUsageMaskBits::E_COLOR_ATTACHMENT_BIT;
  ImageCreateInformation image_ci;
  image_ci.format_ = format;
  image_ci.extent_ = {width, height, 1};
  image_ci.usage_ = usage;
  image_ci.image_type_ = Walle::ImageType::E_2D;
  image_ci.image_view_type_ = Walle::ImageViewType::E_2D;
  return image_ci;
}

ImageCreateInformation ImageCreateInformation::CreateTexture2D(uint32_t width, uint32_t height, Walle::Format format, uint32_t levels) {
  ImageCreateInformation image_ci;
  image_ci.levels_ = levels;
  image_ci.format_ = format;
  image_ci.extent_ = {width, height, 1};
  image_ci.usage_ = Walle::ImageUsageMaskBits::E_SAMPLED_BIT | Walle::ImageUsageMaskBits::E_TRANSFER_DST_BIT;
  image_ci.image_type_ = Walle::ImageType::E_2D;
  image_ci.image_view_type_ = Walle::ImageViewType::E_2D;
  return image_ci;
}

Walle::ImageAspectMask GetAspectMask(Walle::Format format) {
  switch (format) {
  case Walle::Format::E_D16_UNORM:
  case Walle::Format::E_X8_D24_UNORM_PACK32:
  case Walle::Format::E_D32_SFLOAT: return Walle::ImageAspectMaskBits::E_DEPTH_BIT;
  case Walle::Format::E_S8_UINT: return Walle::ImageAspectMaskBits::E_STENCIL_BIT;
  case Walle::Format::E_D16_UNORM_S8_UINT:
  case Walle::Format::E_D24_UNORM_S8_UINT:
  case Walle::Format::E_D32_SFLOAT_S8_UINT: return Walle::ImageAspectMaskBits::E_DEPTH_BIT | Walle::ImageAspectMaskBits::E_STENCIL_BIT;
  default: break;
  }
  return Walle::ImageAspectMaskBits::E_COLOR_BIT;
}

ImageViewCreateInformation ImageViewCreateInformation::CreateDefault(Walle::ImageViewType image_view_type, Walle::Format format) {
  ImageViewCreateInformation image_view_ci;
  image_view_ci.aspect_ = GetAspectMask(format);
  image_view_ci.format_ = format;
  image_view_ci.image_view_type_ = image_view_type;
  return image_view_ci;
}

} // namespace Yuggoth