#include "graphics_types.h"
#include <cassert>

namespace Yuggoth {

ImageAspectMask GetAspectMask(Format format) {
  switch (format) {
  case Format::E_D16_UNORM:
  case Format::E_X8_D24_UNORM_PACK32:
  case Format::E_D32_SFLOAT: return ImageAspectMaskBits::E_DEPTH_BIT;
  case Format::E_S8_UINT: return ImageAspectMaskBits::E_STENCIL_BIT;
  case Format::E_D16_UNORM_S8_UINT:
  case Format::E_D24_UNORM_S8_UINT:
  case Format::E_D32_SFLOAT_S8_UINT: return ImageAspectMaskBits::E_DEPTH_BIT | ImageAspectMaskBits::E_STENCIL_BIT;
  default: break;
  }
  return ImageAspectMaskBits::E_COLOR_BIT;
}

AccessMask2 GetAccessMaskFromLayout(ImageLayout layout, bool destination) {
  switch (layout) {
  case ImageLayout::E_UNDEFINED: assert(destination == false);
  case ImageLayout::E_PRESENT_SRC_KHR: return AccessMaskBits2::E_NONE;
  case ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL: return AccessMaskBits2::E_COLOR_ATTACHMENT_READ_BIT | AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT;
  case ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL:
  case ImageLayout::E_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    return AccessMaskBits2::E_DEPTH_STENCIL_ATTACHMENT_READ_BIT | AccessMaskBits2::E_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  case ImageLayout::E_SHADER_READ_ONLY_OPTIMAL: return AccessMaskBits2::E_SHADER_READ_BIT | AccessMaskBits2::E_INPUT_ATTACHMENT_READ_BIT;
  case ImageLayout::E_TRANSFER_SRC_OPTIMAL: return AccessMaskBits2::E_TRANSFER_READ_BIT;
  case ImageLayout::E_TRANSFER_DST_OPTIMAL: return AccessMaskBits2::E_TRANSFER_WRITE_BIT;
  case ImageLayout::E_PREINITIALIZED: assert(destination == false); return AccessMaskBits2::E_HOST_WRITE_BIT;
  default: std::unreachable();
  }
}

PipelineStageMask2 GetPipelineStageMaskFromLayout(ImageLayout layout, bool destination) {
  switch (layout) {
  case ImageLayout::E_UNDEFINED: return PipelineStageMaskBits2::E_NONE;
  case ImageLayout::E_PREINITIALIZED: return PipelineStageMaskBits2::E_HOST_BIT;
  case ImageLayout::E_TRANSFER_SRC_OPTIMAL:
  case ImageLayout::E_TRANSFER_DST_OPTIMAL: return PipelineStageMaskBits2::E_ALL_TRANSFER_BIT;
  case ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL: return PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT;
  case ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL:
    return PipelineStageMaskBits2::E_EARLY_FRAGMENT_TESTS_BIT | PipelineStageMaskBits2::E_LATE_FRAGMENT_TESTS_BIT;
  case ImageLayout::E_SHADER_READ_ONLY_OPTIMAL: return PipelineStageMaskBits2::E_VERTEX_SHADER_BIT | PipelineStageMaskBits2::E_FRAGMENT_SHADER_BIT;
  case ImageLayout::E_PRESENT_SRC_KHR: return PipelineStageMaskBits2::E_ALL_COMMANDS_BIT;
  default: return destination ? PipelineStageMaskBits2::E_NONE : PipelineStageMaskBits2::E_ALL_COMMANDS_BIT;
  }
}

} // namespace Yuggoth