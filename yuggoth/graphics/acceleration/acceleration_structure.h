#ifndef YUGGOTH_ACCELERATION_STRUCTURE_H
#define YUGGOTH_ACCELERATION_STRUCTURE_H

#include "acceleration_structure_tools.h"
#include <span>

namespace Yuggoth {

class AccelerationStructure {
public:
  AccelerationStructure() = default;

  AccelerationStructure(const BottomLevelGeometry &bottom_geometry);
  AccelerationStructure(std::span<const BLASInstances> blas_instances);

  ~AccelerationStructure();

  AccelerationStructure(const AccelerationStructure &) = delete;
  AccelerationStructure &operator=(const AccelerationStructure &) = delete;

  AccelerationStructure(AccelerationStructure &&other) noexcept;
  AccelerationStructure &operator=(AccelerationStructure &&other) noexcept;

  VkAccelerationStructureKHR GetHandle() const;

protected:
  void BuildBLAS(const BottomLevelGeometry &bottom_geometry, VkBuffer main_buffer, std::size_t main_offset);
  void BuildTLAS(std::span<const BLASInstances> blas_instances, VkBuffer main_buffer, std::size_t main_offset);

private:
  VkAccelerationStructureKHR acceleration_structure_{VK_NULL_HANDLE};
  VkBuffer acceleration_buffer_{VK_NULL_HANDLE};
  VmaAllocation buffer_allocation_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_ACCELERATION_STRUCTURE_H