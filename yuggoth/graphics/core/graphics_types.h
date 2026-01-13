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
inline constexpr BufferUsageMask BUFFER_USAGE_GPU = Walle::BufferUsageMaskBits::E_TRANSFER_DST_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_DEVICE_ADDRESS = Walle::BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_SBT = BUFFER_USAGE_DEVICE_ADDRESS | BUFFER_USAGE_GPU | BufferUsageMaskBits::E_SHADER_BINDING_TABLE_BIT_KHR;
inline constexpr BufferUsageMask BUFFER_USAGE_SCRATCH_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_STORAGE_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
inline constexpr BufferUsageMask BUFFER_USAGE_SOURCES_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
inline constexpr ShaderStageMask SHADER_MASK_GENERAL = ShaderStageMaskBits::E_RAYGEN_BIT_KHR | ShaderStageMaskBits::E_MISS_BIT_KHR | ShaderStageMaskBits::E_CALLABLE_BIT_KHR;
inline constexpr ShaderStageMask SHADER_MAKS_HIT = ShaderStageMaskBits::E_ANY_HIT_BIT_KHR | ShaderStageMaskBits::E_CLOSEST_HIT_BIT_KHR | ShaderStageMaskBits::E_INTERSECTION_BIT_KHR;
// clang-format on

} // namespace CommonMasks

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TYPES_H