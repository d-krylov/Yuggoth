#include "acceleration_structure_tools.h"

namespace Yuggoth {

AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(uint32_t instances) {
  auto device = GraphicsContext::Get()->GetDevice();
  AccelerationStructureBuildSizesInfoKHR out_size;
  AccelerationStructureGeometryKHR geometry;
  geometry.geometryType = GeometryTypeKHR::E_INSTANCES_KHR;
  geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
  AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.type = AccelerationStructureTypeKHR::E_TOP_LEVEL_KHR;
  geometry_bi.geometryCount = 1;
  geometry_bi.pGeometries = &geometry;
  vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, geometry_bi, &instances, out_size);
  return out_size;
}

AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(const BottomLevelGeometry &bottom_geometry) {
  auto device = GraphicsContext::Get()->GetDevice();
  auto geometries = bottom_geometry.GetGeometries();
  AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.type = AccelerationStructureTypeKHR::E_BOTTOM_LEVEL_KHR;
  geometry_bi.geometryCount = geometries.size();
  geometry_bi.pGeometries = geometries.data();
  auto primitives = bottom_geometry.GetPrimitiveCounts();
  AccelerationStructureBuildSizesInfoKHR out_size;
  vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, geometry_bi, primitives.data(), out_size);
  return out_size;
}

VkAccelerationStructureKHR CreateAccelerationStructure(VkBuffer buffer, AccelerationStructureTypeKHR type, std::size_t offset, std::size_t size) {
  AccelerationStructureCreateInfoKHR acceleration_structure_ci;
  acceleration_structure_ci.buffer = buffer;
  acceleration_structure_ci.offset = offset;
  acceleration_structure_ci.size = size;
  acceleration_structure_ci.type = type;
  VkAccelerationStructureKHR acceleration_structure = VK_NULL_HANDLE;
  VK_CHECK(vkCreateAccelerationStructureKHR(GraphicsContext::Get()->GetDevice(), acceleration_structure_ci, nullptr, &acceleration_structure));
  return acceleration_structure;
}

AccelerationStructureBuildGeometryInfoKHR GetBuildGeometryInformation(std::span<const AccelerationStructureGeometryKHR> geometries,
                                                                      VkDeviceAddress scratch_buffer,
                                                                      VkAccelerationStructureKHR acceleration_structure,
                                                                      AccelerationStructureTypeKHR type) {
  AccelerationStructureBuildGeometryInfoKHR geometry_bi;
  geometry_bi.flags = BuildAccelerationStructureMaskBitsKHR::E_PREFER_FAST_TRACE_BIT_KHR;
  geometry_bi.mode = BuildAccelerationStructureModeKHR::E_BUILD_KHR;
  geometry_bi.type = type;
  geometry_bi.geometryCount = geometries.size();
  geometry_bi.pGeometries = geometries.data();
  geometry_bi.dstAccelerationStructure = acceleration_structure;
  geometry_bi.scratchData.deviceAddress = scratch_buffer;
  return geometry_bi;
}

} // namespace Yuggoth