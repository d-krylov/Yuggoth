#ifndef YUGGOTH_ACCELERATION_STRUCTURE_TOOLS_H
#define YUGGOTH_ACCELERATION_STRUCTURE_TOOLS_H

#include "yuggoth/graphics_device/graphics_device.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include "bottom_level_geometry.h"

namespace Yuggoth {

struct InstanceData {
  Matrix4f transform_{1.0f};
};

struct BLASInstances {
  std::vector<InstanceData> instances_;
  VkAccelerationStructureKHR acceleration_structure_{VK_NULL_HANDLE};
};

AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(uint32_t instances);
AccelerationStructureBuildSizesInfoKHR GetAccelerationStructureSize(const BottomLevelGeometry &bottom_geometry);
VkAccelerationStructureKHR CreateAccelerationStructure(VkBuffer buffer, AccelerationStructureTypeKHR type, std::size_t offset, std::size_t size);
AccelerationStructureBuildGeometryInfoKHR GetBuildGeometryInformation(std::span<const AccelerationStructureGeometryKHR> geometries,
                                                                      VkDeviceAddress scratch_buffer,
                                                                      VkAccelerationStructureKHR acceleration_structure,
                                                                      AccelerationStructureTypeKHR type);

} // namespace Yuggoth

#endif // YUGGOTH_ACCELERATION_STRUCTURE_TOOLS_H