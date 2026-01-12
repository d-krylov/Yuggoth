#ifndef YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H
#define YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H

#include "yuggoth/graphics_device/graphics_device.h"
#include "yuggoth/core/tools/include/core_types.h"
#include <span>

namespace Yuggoth {

class BottomLevelGeometry {
public:
  void AddTriangleGeometry(VkDeviceAddress vbo_device_address, const ElementRange &vbo_range, //
                           VkDeviceAddress ibo_device_address, const ElementRange &ibo_range, //
                           GeometryMaskKHR mask);

  std::span<const AccelerationStructureGeometryKHR> GetGeometries() const;
  std::span<const AccelerationStructureBuildRangeInfoKHR> GetRanges() const;

  std::vector<uint32_t> GetPrimitiveCounts() const;

  std::size_t GetGeometryCount() const;

private:
  std::vector<AccelerationStructureGeometryKHR> geometries_;
  std::vector<AccelerationStructureBuildRangeInfoKHR> ranges_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H