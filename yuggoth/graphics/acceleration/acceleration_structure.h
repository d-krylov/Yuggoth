#ifndef YUGGOTH_ACCELERATION_STRUCTURE_H
#define YUGGOTH_ACCELERATION_STRUCTURE_H

#include "acceleration_structure_tools.h"
#include <span>

namespace Yuggoth {

struct AccelerationSpecification {
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkDeviceAddress scratch_address_ = 0;
  std::size_t buffer_offset_ = 0;
  std::size_t structure_size_ = 0;
  std::size_t primitive_count_ = 0;
};

class AccelerationStructure {
public:
  AccelerationStructure() = default;

  AccelerationStructure(const BottomLevelGeometry &bottom_geometry);
  AccelerationStructure(std::span<const BLASInstances> bottom_instances);

  ~AccelerationStructure();

  AccelerationStructure(const AccelerationStructure &) = delete;
  AccelerationStructure &operator=(const AccelerationStructure &) = delete;

  AccelerationStructure(AccelerationStructure &&other) noexcept;
  AccelerationStructure &operator=(AccelerationStructure &&other) noexcept;

  VkAccelerationStructureKHR GetHandle() const;

protected:
  void BuildBLAS(const BottomLevelGeometry &bottom_geometry, const AccelerationSpecification &specification);

  void BuildTLAS(std::span<const BLASInstances> blas_instances, const AccelerationSpecification &specification);

private:
  VkAccelerationStructureKHR acceleration_structure_{VK_NULL_HANDLE};
  VkBuffer acceleration_buffer_{VK_NULL_HANDLE};
  VmaAllocation buffer_allocation_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_ACCELERATION_STRUCTURE_H