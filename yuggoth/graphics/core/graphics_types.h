#ifndef YUGGOTH_GRAPHICS_TYPES_H
#define YUGGOTH_GRAPHICS_TYPES_H

#include "structure_tools.h"
#include "yuggoth/core/allocators/virtual_allocator_types.h"

namespace Yuggoth {

ImageAspectMask GetAspectMask(Format format);
AccessMask2 GetAccessMaskFromLayout(ImageLayout layout, bool destination);
PipelineStageMask2 GetPipelineStageMaskFromLayout(ImageLayout layout, bool destination);

namespace CommonMasks {

// clang-format off
inline constexpr AllocationCreateMask BUFFER_GPU = AllocationCreateMaskBits::E_DEDICATED_MEMORY_BIT;
inline constexpr AllocationCreateMask BUFFER_CPU = AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
inline constexpr AllocationCreateMask BUFFER_MAPPED = BUFFER_CPU | AllocationCreateMaskBits::E_MAPPED_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_GPU = BufferUsageMaskBits::E_TRANSFER_DST_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_DEVICE_ADDRESS = BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_SCRATCH_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT;
inline constexpr BufferUsageMask BUFFER_USAGE_STORAGE_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
inline constexpr BufferUsageMask BUFFER_USAGE_SOURCES_AS = BUFFER_USAGE_DEVICE_ADDRESS | BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
// clang-format on

} // namespace CommonMasks

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TYPES_H