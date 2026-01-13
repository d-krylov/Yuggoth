#include "graphics_types.h"
#include <cassert>

namespace Yuggoth {

AccessMask2 GetAccessMaskFromLayout(Walle::ImageLayout layout, bool destination) {
  switch (layout) {
  case Walle::ImageLayout::E_UNDEFINED: assert(destination == false);
  case Walle::ImageLayout::E_PRESENT_SRC_KHR: return Walle::AccessMaskBits2::E_NONE;
  case Walle::ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL:
    return Walle::AccessMaskBits2::E_COLOR_ATTACHMENT_READ_BIT | Walle::AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT;
  case Walle::ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL:
  case Walle::ImageLayout::E_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    return Walle::AccessMaskBits2::E_DEPTH_STENCIL_ATTACHMENT_READ_BIT | Walle::AccessMaskBits2::E_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  case Walle::ImageLayout::E_SHADER_READ_ONLY_OPTIMAL: return Walle::AccessMaskBits2::E_SHADER_READ_BIT | AccessMaskBits2::E_INPUT_ATTACHMENT_READ_BIT;
  case Walle::ImageLayout::E_TRANSFER_SRC_OPTIMAL: return Walle::AccessMaskBits2::E_TRANSFER_READ_BIT;
  case Walle::ImageLayout::E_TRANSFER_DST_OPTIMAL: return Walle::AccessMaskBits2::E_TRANSFER_WRITE_BIT;
  case Walle::ImageLayout::E_PREINITIALIZED: assert(destination == false); return Walle::AccessMaskBits2::E_HOST_WRITE_BIT;
  case Walle::ImageLayout::E_GENERAL: return Walle::AccessMaskBits2::E_SHADER_WRITE_BIT;
  default: std::unreachable();
  }
}

PipelineStageMask2 GetPipelineStageMaskFromLayout(Walle::ImageLayout layout, bool destination) {
  switch (layout) {
  case ImageLayout::E_UNDEFINED: return PipelineStageMaskBits2::E_NONE;
  case ImageLayout::E_PREINITIALIZED: return PipelineStageMaskBits2::E_HOST_BIT;
  case ImageLayout::E_TRANSFER_SRC_OPTIMAL:
  case ImageLayout::E_TRANSFER_DST_OPTIMAL: return PipelineStageMaskBits2::E_ALL_TRANSFER_BIT;
  case ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL: return PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT;
  case ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL: return PipelineStageMaskBits2::E_EARLY_FRAGMENT_TESTS_BIT | PipelineStageMaskBits2::E_LATE_FRAGMENT_TESTS_BIT;
  case ImageLayout::E_SHADER_READ_ONLY_OPTIMAL: return PipelineStageMaskBits2::E_VERTEX_SHADER_BIT | PipelineStageMaskBits2::E_FRAGMENT_SHADER_BIT;
  case ImageLayout::E_PRESENT_SRC_KHR: return PipelineStageMaskBits2::E_ALL_COMMANDS_BIT;
  case ImageLayout::E_GENERAL: return PipelineStageMaskBits2::E_RAY_TRACING_SHADER_BIT_KHR | PipelineStageMaskBits2::E_COMPUTE_SHADER_BIT;
  default: return destination ? PipelineStageMaskBits2::E_NONE : PipelineStageMaskBits2::E_ALL_COMMANDS_BIT;
  }
}

PipelineStageMask2 StageMaskFromBufferUsage(Walle::BufferUsageMask usage) {
  PipelineStageMask2 mask;
  BufferUsageMask vertices_mask = BufferUsageMaskBits::E_VERTEX_BUFFER_BIT | BufferUsageMaskBits::E_INDEX_BUFFER_BIT;
  BufferUsageMask acceleration_mask = BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
  BufferUsageMask indirect_mask = BufferUsageMaskBits::E_INDIRECT_BUFFER_BIT;
  BufferUsageMask common_mask = BufferUsageMaskBits::E_STORAGE_BUFFER_BIT | BufferUsageMaskBits::E_UNIFORM_BUFFER_BIT;

  PipelineStageMaskBits2::E_ALL_GRAPHICS_BIT | PipelineStageMaskBits2::E_COMPUTE_SHADER_BIT;

  mask |= usage.HasAnyBits(vertices_mask) ? PipelineStageMaskBits2::E_VERTEX_INPUT_BIT : PipelineStageMask2();
  mask |= usage.HasBits(acceleration_mask) ? PipelineStageMaskBits2::E_ACCELERATION_STRUCTURE_BUILD_BIT_KHR : PipelineStageMask2();
  mask |= usage.HasBits(indirect_mask) ? PipelineStageMaskBits2::E_DRAW_INDIRECT_BIT : PipelineStageMask2();
}

uint32_t GetImageNumberMipLevels(const Walle::Extent3D &extent) {
  auto size = std::max<uint32_t>(std::max<uint32_t>(extent.width, extent.height), extent.depth);
  return std::bit_width(size);
}

} // namespace Yuggoth