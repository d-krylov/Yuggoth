#include "bottom_level_geometry.h"
#include <algorithm>

namespace Yuggoth {

void BottomLevelGeometry::AddTriangleGeometry(VkDeviceAddress vbo_device_address, const ElementRange &vbo_range, //
                                              VkDeviceAddress ibo_device_address, const ElementRange &ibo_range, //
                                              GeometryMaskKHR mask) {
  AccelerationStructureGeometryKHR geometry;
  AccelerationStructureBuildRangeInfoKHR range;
  geometry.geometryType = GeometryTypeKHR::E_TRIANGLES_KHR;
  geometry.flags = mask;
  geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
  geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
  geometry.geometry.triangles.vertexData.deviceAddress = vbo_device_address;
  geometry.geometry.triangles.maxVertex = vbo_range.count_ - 1;
  geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
  geometry.geometry.triangles.indexData.deviceAddress = ibo_device_address;
  geometry.geometry.triangles.vertexStride = vbo_range.stride_;

  range.firstVertex = vbo_range.offset_;
  range.primitiveOffset = 0;
  range.primitiveCount = ibo_range.count_ / 3;

  geometries_.emplace_back(geometry);
  ranges_.emplace_back(range);
}

std::span<const AccelerationStructureGeometryKHR> BottomLevelGeometry::GetGeometries() const {
  return geometries_;
}

std::span<const AccelerationStructureBuildRangeInfoKHR> BottomLevelGeometry::GetRanges() const {
  return ranges_;
}

std::size_t BottomLevelGeometry::GetGeometryCount() const {
  return geometries_.size();
}

std::vector<uint32_t> BottomLevelGeometry::GetPrimitiveCounts() const {
  std::vector<uint32_t> counts;
  std::ranges::transform(ranges_, std::back_inserter(counts), std::identity(), &AccelerationStructureBuildRangeInfoKHR::primitiveCount);
  return counts;
}

} // namespace Yuggoth