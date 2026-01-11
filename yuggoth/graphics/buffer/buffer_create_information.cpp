#include "buffer_create_information.h"

namespace Yuggoth {

BufferCreateInformation BufferCreateInformation::CreateGPUBuffer(uint64_t size, Walle::BufferUsageMask usage) {
  BufferCreateInformation buffer_ci;
  buffer_ci.buffer_size_ = size;
  buffer_ci.buffer_usage_ = usage | Walle::BufferUsageMaskBits::E_TRANSFER_DST_BIT;
  buffer_ci.required_memory_property_ = Walle::MemoryPropertyMaskBits::E_DEVICE_LOCAL_BIT;
  return buffer_ci;
}

BufferCreateInformation BufferCreateInformation::CreateCPUBuffer(uint64_t size, Walle::BufferUsageMask usage, bool mapped) {
  BufferCreateInformation buffer_ci;
  buffer_ci.buffer_size_ = size;
  buffer_ci.buffer_usage_ = usage;
  buffer_ci.allocator_mask_ = mapped ? AllocationCreateMaskBits::E_MAPPED_BIT : AllocationCreateMask();
  buffer_ci.required_memory_property_ = Walle::MemoryPropertyMaskBits::E_HOST_VISIBLE_BIT;
  return buffer_ci;
}

BufferCreateInformation BufferCreateInformation::CreateAccelerationStructureBuffer(uint64_t size) {
  BufferCreateInformation buffer_ci;
  buffer_ci.buffer_size_ = size;
  buffer_ci.buffer_usage_ |= Walle::BufferUsageMaskBits::E_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
  buffer_ci.buffer_usage_ |= Walle::BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT;
  buffer_ci.required_memory_property_ = Walle::MemoryPropertyMaskBits::E_DEVICE_LOCAL_BIT;
  return buffer_ci;
}

BufferCreateInformation BufferCreateInformation::CreateStagingBuffer(uint64_t size) {
  return BufferCreateInformation::CreateCPUBuffer(size, Walle::BufferUsageMaskBits::E_TRANSFER_SRC_BIT);
}

} // namespace Yuggoth