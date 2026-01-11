#include "image_create_information.h"

namespace Yuggoth {

ImageCreateInformation ImageCreateInformation::CreateRenderTarget(uint32_t width, uint32_t height, Walle::Format format) {
  auto usage = Walle::IsFormatDepthOrStencil(format) ? Walle::ImageUsageMaskBits::E_DEPTH_STENCIL_ATTACHMENT_BIT
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

} // namespace Yuggoth