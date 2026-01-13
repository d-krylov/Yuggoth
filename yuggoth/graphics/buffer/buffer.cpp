#include "buffer.h"
#include "yuggoth/graphics/core/graphics_allocator.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include <algorithm>

namespace Yuggoth {

// CONSTRUCTORS

Buffer::Buffer(const BufferCreateInformation &buffer_ci) : buffer_size_(buffer_ci.buffer_size_), buffer_usage_(buffer_ci.buffer_usage_) {
  auto buffer_information = CreateBuffer(buffer_ci);
  buffer_ = buffer_information.buffer_handle_;
  allocation_ = buffer_information.allocation_;
  memory_type_ = buffer_information.memory_type_;
  mapped_memory_ = buffer_information.mapped_memory_;
  memory_property_ = buffer_information.memory_property_;
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

BufferAllocationInformation Buffer::CreateBuffer(const BufferCreateInformation &buffer_create_information) {
  Walle::BufferCreateInfo buffer_ci;
  buffer_ci.size = buffer_create_information.buffer_size_;
  buffer_ci.usage = buffer_create_information.buffer_usage_;
  buffer_ci.sharingMode = Walle::SharingMode::E_EXCLUSIVE;
  AllocationCreateInformation allocation_ci;
  allocation_ci.allocation_create_mask_ = buffer_create_information.allocator_mask_;
  allocation_ci.required_memory_property_ = buffer_create_information.required_memory_property_;
  allocation_ci.preferred_memory_property_ = buffer_create_information.preferred_memory_property_;
  auto buffer_information = GraphicsAllocator::Get()->AllocateBuffer(buffer_ci, allocation_ci);
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

void Buffer::Resize(std::size_t size) {
}

// API

VkDeviceAddress Buffer::GetDeviceAddress() const {
  auto handle = GetHandle();
  return GetBufferDeviceAddress(handle);
}

VkDeviceAddress Buffer::GetBufferDeviceAddress(VkBuffer buffer) {
  Walle::BufferDeviceAddressInfo buffer_device_ai;
  buffer_device_ai.buffer = buffer;
  auto address = vkGetBufferDeviceAddress(GraphicsContext::Get()->GetDevice(), buffer_device_ai);
  return address;
}

void Buffer::SetRawData(std::span<const std::byte> data, std::size_t byte_offset) {
  GraphicsAllocator::Get()->CopyMemoryToAllocation(data, allocation_, byte_offset);
}

std::size_t Buffer::GetSize() const {
  return buffer_size_;
}

VkBuffer Buffer::GetHandle() const {
  return buffer_;
}

Walle::BufferUsageMask Buffer::GetUsage() const {
  return buffer_usage_;
}

Walle::MemoryPropertyMask Buffer::GetMemoryPropertyMask() const {
  return memory_property_;
}

bool Buffer::IsGPU() const {
  return memory_property_.HasBits(Walle::MemoryPropertyMaskBits::E_DEVICE_LOCAL_BIT);
}

bool Buffer::IsAccessWithCPU() const {
  return memory_property_.HasBits(Walle::MemoryPropertyMaskBits::E_HOST_VISIBLE_BIT);
}

} // namespace Yuggoth