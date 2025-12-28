#include "acceleration_structure.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"

namespace Yuggoth {

BufferUsageMask blas_usage = BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT;

void AccelerationStructure::BuildBLAS(const BottomLevelGeometry &bottom_geometry, VkBuffer main_buffer, std::size_t offset) {
  auto acceleration_structure_sizes = GetAccelerationStructureSize(bottom_geometry);
  auto main_size = acceleration_structure_sizes.accelerationStructureSize;
  auto main_buffer_information = Buffer::CreateBuffer(main_size, blas_usage, AllocationCreateMaskBits::E_DEDICATED_MEMORY_BIT);
  acceleration_buffer_ = main_buffer_information.first;
  buffer_allocation_ = main_buffer_information.second.allocation_;
  Buffer scrath_buffer(acceleration_structure_sizes.buildScratchSize, CommonMasks::BUFFER_USAGE_SCRATCH_AS, AllocationCreateMask());

  auto sizes = GetAccelerationStructureSize(bottom_geometry);
  auto type = AccelerationStructureTypeKHR::E_BOTTOM_LEVEL_KHR;

  auto geometries = bottom_geometry.GetGeometries();

  VkAccelerationStructureKHR acceleration_structure = VK_NULL_HANDLE;
  acceleration_structure = CreateAccelerationStructure(main_buffer, type, offset, sizes.accelerationStructureSize);
  auto geometry_info = GetBuildGeometryInformation(geometries, scrath_buffer.GetBufferDeviceAddress(), acceleration_structure, type);

  auto build_ranges = bottom_geometry.GetRanges();
  auto build_range_pointer = reinterpret_cast<const VkAccelerationStructureBuildRangeInfoKHR *>(build_ranges.data());

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  vkCmdBuildAccelerationStructuresKHR(command_buffer.GetHandle(), 1, geometry_info, &build_range_pointer);
  command_buffer.End();
  command_buffer.Submit();
}

} // namespace Yuggoth