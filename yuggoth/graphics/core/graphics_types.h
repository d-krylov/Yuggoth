#ifndef YUGGOTH_GRAPHICS_TYPES_H
#define YUGGOTH_GRAPHICS_TYPES_H

#include "structure_tools.h"
#include <volk/volk.h>

namespace Yuggoth {

ImageAspectMask GetAspectMask(Walle::Format format);
AccessMask2 GetAccessMaskFromLayout(Walle::ImageLayout layout, bool destination);
PipelineStageMask2 GetPipelineStageMaskFromLayout(Walle::ImageLayout layout, bool destination);

namespace CommonMasks {

// clang-format off
inline constexpr BufferUsageMask BUFFER_USAGE_DEVICE_ADDRESS = Walle::BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_SCRATCH_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_SOURCES_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
// clang-format on

} // namespace CommonMasks

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TYPES_H