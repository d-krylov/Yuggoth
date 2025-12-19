#include "acceleration_structure.h"

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

} // namespace Yuggoth