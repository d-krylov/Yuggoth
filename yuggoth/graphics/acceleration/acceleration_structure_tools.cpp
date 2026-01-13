#include "acceleration_structure_tools.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Walle::AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(uint32_t instances) {
  auto device = GraphicsContext::Get()->GetDevice();
  Walle::AccelerationStructureBuildSizesInfoKHR out_size;
  Walle::AccelerationStructureGeometryKHR geometry;
  geometry.geometryType = Walle::GeometryTypeKHR::E_INSTANCES_KHR;
  geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
  Walle::AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = Walle::BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.type = Walle::AccelerationStructureTypeKHR::E_TOP_LEVEL_KHR;
  geometry_bi.geometryCount = 1;
  geometry_bi.pGeometries = &geometry;
  vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, geometry_bi, &instances, out_size);
  return out_size;
}

Walle::AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(const BottomLevelGeometry &bottom_geometry) {
  auto device = GraphicsContext::Get()->GetDevice();
  auto geometries = bottom_geometry.GetGeometries();
  Walle::AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = Walle::BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.type = Walle::AccelerationStructureTypeKHR::E_BOTTOM_LEVEL_KHR;
  geometry_bi.geometryCount = geometries.size();
  geometry_bi.pGeometries = geometries.data();
  auto primitives = bottom_geometry.GetPrimitiveCounts();
  Walle::AccelerationStructureBuildSizesInfoKHR out_size;
  vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, geometry_bi, primitives.data(), out_size);
  return out_size;
}

VkAccelerationStructureKHR CreateAccelerationStructure(VkBuffer buffer, Walle::AccelerationStructureTypeKHR type, std::size_t offset, std::size_t size) {
  Walle::AccelerationStructureCreateInfoKHR acceleration_structure_ci;
  acceleration_structure_ci.buffer = buffer;
  acceleration_structure_ci.offset = offset;
  acceleration_structure_ci.size = size;
  acceleration_structure_ci.type = type;
  VkAccelerationStructureKHR acceleration_structure = VK_NULL_HANDLE;
  VK_CHECK(vkCreateAccelerationStructureKHR(GraphicsContext::Get()->GetDevice(), acceleration_structure_ci, nullptr, &acceleration_structure));
  return acceleration_structure;
}

Walle::AccelerationStructureBuildGeometryInfoKHR GetBuildGeometryInformation(std::span<const Walle::AccelerationStructureGeometryKHR> geometries,
                                                                             VkDeviceAddress scratch_buffer, VkAccelerationStructureKHR acceleration_structure,
                                                                             Walle::AccelerationStructureTypeKHR type) {
  Walle::AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = Walle::BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.mode = Walle::BuildAccelerationStructureModeKHR::E_BUILD_KHR;
  geometry_bi.type = type;
  geometry_bi.geometryCount = geometries.size();
  geometry_bi.pGeometries = geometries.data();
  geometry_bi.dstAccelerationStructure = acceleration_structure;
  geometry_bi.scratchData.deviceAddress = scratch_buffer;
  return geometry_bi;
}

} // namespace Yuggoth