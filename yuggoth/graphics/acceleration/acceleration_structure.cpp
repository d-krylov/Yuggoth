#include "acceleration_structure.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"

namespace Yuggoth {

AccelerationStructure::AccelerationStructure(AccelerationStructure &&other) noexcept {
  acceleration_structure_ = std::exchange(other.acceleration_structure_, VK_NULL_HANDLE);
  acceleration_buffer_ = std::exchange(other.acceleration_buffer_, VK_NULL_HANDLE);
  buffer_allocation_ = std::exchange(other.buffer_allocation_, VK_NULL_HANDLE);
}

AccelerationStructure::~AccelerationStructure() {
  vkDestroyAccelerationStructureKHR(GraphicsContext::Get()->GetDevice(), acceleration_structure_, nullptr);
  GraphicsAllocator::Get()->DestroyBuffer(acceleration_buffer_, buffer_allocation_);
}

AccelerationStructure &AccelerationStructure::operator=(AccelerationStructure &&other) noexcept {
  std::swap(acceleration_structure_, other.acceleration_structure_);
  std::swap(acceleration_buffer_, other.acceleration_buffer_);
  std::swap(buffer_allocation_, other.buffer_allocation_);
  return *this;
}

AccelerationStructure::AccelerationStructure(const BottomLevelGeometry &bottom_geometry) {
}

AccelerationStructure::AccelerationStructure(std::span<const BLASInstances> blas_instances) {
}

VkAccelerationStructureKHR AccelerationStructure::GetHandle() const {
  return acceleration_structure_;
}

} // namespace Yuggoth