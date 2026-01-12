#include "acceleration_structure.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"

namespace Yuggoth {

void AccelerationStructure::BuildBLAS(const BottomLevelGeometry &bottom_geometry, const AccelerationSpecification &specification) {
  auto type = AccelerationStructureTypeKHR::E_BOTTOM_LEVEL_KHR;

  auto geometries = bottom_geometry.GetGeometries();

  acceleration_structure_ = CreateAccelerationStructure(specification.buffer_, type, specification.buffer_offset_, specification.structure_size_);
  auto geometry_info = GetBuildGeometryInformation(geometries, specification.scratch_address_, acceleration_structure_, type);

  auto build_ranges = bottom_geometry.GetRanges();
  auto build_range_pointer = reinterpret_cast<const VkAccelerationStructureBuildRangeInfoKHR *>(build_ranges.data());

  CommandBuffer command_buffer(GraphicsDevice::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  vkCmdBuildAccelerationStructuresKHR(command_buffer.GetHandle(), 1, geometry_info, &build_range_pointer);
  command_buffer.End();
  command_buffer.Submit();
}

} // namespace Yuggoth