#include "acceleration_structure.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include <algorithm>

namespace Yuggoth {

AccelerationStructure::AccelerationStructure(const BottomLevelGeometry &bottom_geometry) {
  auto acceleration_structure_sizes = GetAccelerationStructureSize(bottom_geometry);
  auto main_size = acceleration_structure_sizes.accelerationStructureSize;
  auto buffer_information = Buffer::CreateBuffer(main_size, CommonMasks::BUFFER_USAGE_STORAGE_AS, CommonMasks::BUFFER_GPU);
  acceleration_buffer_ = buffer_information.first;
  buffer_allocation_ = buffer_information.second.allocation_;
  Buffer scratch_buffer(acceleration_structure_sizes.buildScratchSize, CommonMasks::BUFFER_USAGE_SCRATCH_AS, CommonMasks::BUFFER_GPU);
  AccelerationSpecification specification;
  specification.buffer_ = acceleration_buffer_;
  specification.scratch_address_ = scratch_buffer.GetDeviceAddress();
  specification.structure_size_ = main_size;
  BuildBLAS(bottom_geometry, specification);
}

AccelerationStructure::AccelerationStructure(std::span<const BLASInstances> bottom_instances) {
  uint32_t primitive_count = 0;
  std::ranges::for_each(bottom_instances, [&](const auto &instances) { primitive_count += instances.size(); }, &BLASInstances::instances_);
  auto acceleration_structure_sizes = GetAccelerationStructureSize(primitive_count);
  auto main_size = acceleration_structure_sizes.accelerationStructureSize;
  auto buffer_information = Buffer::CreateBuffer(main_size, CommonMasks::BUFFER_USAGE_STORAGE_AS, CommonMasks::BUFFER_GPU);
  acceleration_buffer_ = buffer_information.first;
  buffer_allocation_ = buffer_information.second.allocation_;
  Buffer scratch_buffer(acceleration_structure_sizes.buildScratchSize, CommonMasks::BUFFER_USAGE_SCRATCH_AS, CommonMasks::BUFFER_GPU);
  AccelerationSpecification specification;
  specification.buffer_ = acceleration_buffer_;
  specification.scratch_address_ = scratch_buffer.GetDeviceAddress();
  specification.structure_size_ = main_size;
  specification.primitive_count_ = primitive_count;
  BuildTLAS(bottom_instances, specification);
}

AccelerationStructure::~AccelerationStructure() {
  vkDestroyAccelerationStructureKHR(GraphicsContext::Get()->GetDevice(), acceleration_structure_, nullptr);
  if (buffer_allocation_ != nullptr && acceleration_buffer_ != nullptr) {
    GraphicsAllocator::Get()->DestroyBuffer(acceleration_buffer_, buffer_allocation_);
  }
}

AccelerationStructure::AccelerationStructure(AccelerationStructure &&other) noexcept {
  acceleration_structure_ = std::exchange(other.acceleration_structure_, VK_NULL_HANDLE);
  acceleration_buffer_ = std::exchange(other.acceleration_buffer_, VK_NULL_HANDLE);
  buffer_allocation_ = std::exchange(other.buffer_allocation_, VK_NULL_HANDLE);
}

AccelerationStructure &AccelerationStructure::operator=(AccelerationStructure &&other) noexcept {
  std::swap(acceleration_structure_, other.acceleration_structure_);
  std::swap(acceleration_buffer_, other.acceleration_buffer_);
  std::swap(buffer_allocation_, other.buffer_allocation_);
  return *this;
}

VkAccelerationStructureKHR AccelerationStructure::GetHandle() const {
  return acceleration_structure_;
}

} // namespace Yuggoth