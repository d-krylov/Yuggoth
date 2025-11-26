#include "buffer.h"

namespace Yuggoth {

// CONSTRUCTORS

Buffer::Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage, AllocationCreateMask allocation_mask) : buffer_size_(buffer_size) {
  CreateBuffer(allocation_mask, buffer_size, buffer_usage);
}

Buffer::~Buffer() {
}

void Buffer::CreateBuffer(AllocationCreateMask allocation_mask, std::size_t buffer_size, BufferUsageMask buffer_usage) {
  BufferCreateInfo buffer_ci;
  buffer_ci.size = buffer_size;
  buffer_ci.usage = buffer_usage;
  buffer_ci.sharingMode = SharingMode::E_EXCLUSIVE;

  auto allocation_information = GraphicsAllocator::Get()->AllocateBuffer(buffer_ci, buffer_, allocation_mask);

  allocation_ = allocation_information.allocation_;
  mapped_memory_ = allocation_information.mapped_memory_;
}

Buffer::Buffer(Buffer &&other) noexcept {
  buffer_ = std::exchange(other.buffer_, VK_NULL_HANDLE);
  allocation_ = std::exchange(other.allocation_, VK_NULL_HANDLE);
}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  std::swap(buffer_, other.buffer_);
  std::swap(allocation_, other.allocation_);
  std::swap(mapped_memory_, other.mapped_memory_);
  std::swap(buffer_size_, other.buffer_size_);
  return *this;
}

std::size_t Buffer::GetSize() const {
  return buffer_size_;
}

VkBuffer Buffer::GetHandle() const {
  return buffer_;
}

void Buffer::Map() {
  GraphicsAllocator::Get()->MapMemory(allocation_, &mapped_memory_);
}

void Buffer::Unmap() {
  GraphicsAllocator::Get()->UnmapMemory(allocation_);
  mapped_memory_ = nullptr;
}

VkDeviceAddress Buffer::GetBufferDeviceAddress() const {
  BufferDeviceAddressInfo buffer_device_ai;
  buffer_device_ai.buffer = GetHandle();
  auto address = vkGetBufferDeviceAddress(GraphicsContext::Get()->GetDevice(), buffer_device_ai);
  return address;
}

} // namespace Yuggoth