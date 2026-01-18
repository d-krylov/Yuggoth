#ifndef YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H
#define YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H

#include <yuggoth/core/tools/tools.h>
#include "walle/walle.h"
#include <vector>
#include <span>

namespace Yuggoth {

class BottomLevelGeometry {
public:
  void AddTriangleGeometry(VkDeviceAddress vbo_device_address, const ElementRange &vbo_range, //
                           VkDeviceAddress ibo_device_address, const ElementRange &ibo_range, //
                           Walle::GeometryMaskKHR mask);

  std::span<const Walle::AccelerationStructureGeometryKHR> GetGeometries() const;
  std::span<const Walle::AccelerationStructureBuildRangeInfoKHR> GetRanges() const;

  std::vector<uint32_t> GetPrimitiveCounts() const;

  std::size_t GetGeometryCount() const;

private:
  std::vector<Walle::AccelerationStructureGeometryKHR> geometries_;
  std::vector<Walle::AccelerationStructureBuildRangeInfoKHR> ranges_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BOTTOM_LEVEL_GEOMETRY_H