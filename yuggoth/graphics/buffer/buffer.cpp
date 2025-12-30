#include "buffer.h"
#include <algorithm>

namespace Yuggoth {

// CONSTRUCTORS

Buffer::Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage, AllocationCreateMask allocation_mask)
  : buffer_size_(buffer_size), buffer_usage_(buffer_usage) {
  auto buffer_information = CreateBuffer(buffer_size, buffer_usage, allocation_mask);
  buffer_ = buffer_information.first;
  allocation_ = buffer_information.second.allocation_;
  memory_type_ = buffer_information.second.memory_type_;
  mapped_memory_ = buffer_information.second.mapped_memory_;
  memory_property_ = buffer_information.second.memory_property_;
}

Buffer::~Buffer() {
  Destroy();
}

void Buffer::Destroy() {
  if (allocation_ == nullptr && buffer_ == nullptr) return;
  GraphicsAllocator::Get()->DestroyBuffer(buffer_, allocation_);
}

Buffer::Buffer(Buffer &&other) noexcept {
  Swap(other);
}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  Swap(other);
  return *this;
}

void Buffer::Swap(Buffer &other) noexcept {
  std::swap(buffer_, other.buffer_);
  std::swap(allocation_, other.allocation_);
  std::swap(mapped_memory_, other.mapped_memory_);
  std::swap(buffer_size_, other.buffer_size_);
  std::swap(buffer_usage_, other.buffer_usage_);
  std::swap(memory_property_, other.memory_property_);
  std::swap(memory_type_, other.memory_type_);
}

BufferInformation Buffer::CreateBuffer(std::size_t size, BufferUsageMask usage, AllocationCreateMask allocation_mask) {
  BufferCreateInfo buffer_ci;
  buffer_ci.size = size;
  buffer_ci.usage = usage;
  buffer_ci.sharingMode = SharingMode::E_EXCLUSIVE;

  BufferInformation buffer_information;
  buffer_information.second = GraphicsAllocator::Get()->AllocateBuffer(buffer_ci, buffer_information.first, allocation_mask);

  return buffer_information;
}

void Buffer::Map() {
  GraphicsAllocator::Get()->MapMemory(allocation_, &mapped_memory_);
}

void Buffer::Unmap() {
  GraphicsAllocator::Get()->UnmapMemory(allocation_);
  mapped_memory_ = nullptr;
}

void Buffer::MemoryCopy(std::span<const std::byte> data, std::size_t byte_offset) {
  auto destination_span = GetMappedAs<std::byte>();
  auto destination_offset = destination_span.subspan(byte_offset);
  std::ranges::copy(data, destination_offset.begin());
}

// GETTERS

VkDeviceAddress Buffer::GetDeviceAddress() const {
  auto handle = GetHandle();
  return GetBufferDeviceAddress(handle);
}

VkDeviceAddress Buffer::GetBufferDeviceAddress(VkBuffer buffer) {
  BufferDeviceAddressInfo buffer_device_ai;
  buffer_device_ai.buffer = buffer;
  auto address = vkGetBufferDeviceAddress(GraphicsContext::Get()->GetDevice(), buffer_device_ai);
  return address;
}

std::size_t Buffer::GetSize() const {
  return buffer_size_;
}

VkBuffer Buffer::GetHandle() const {
  return buffer_;
}

BufferUsageMask Buffer::GetUsage() const {
  return buffer_usage_;
}

MemoryPropertyMask Buffer::GetMemoryPropertyMask() const {
  return memory_property_;
}

bool Buffer::IsGPU() const {
  return memory_property_.HasBits(MemoryPropertyMaskBits::E_DEVICE_LOCAL_BIT);
}

bool Buffer::IsAccessWithCPU() const {
  return memory_property_.HasBits(MemoryPropertyMaskBits::E_HOST_VISIBLE_BIT);
}

} // namespace Yuggoth