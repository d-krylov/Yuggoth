#include "buffer.h"

namespace Yuggoth {

// CONSTRUCTORS

Buffer::Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage, bool mapped) {
  BufferCreateInfo buffer_ci{};
  {
    buffer_ci.size = buffer_size;
    buffer_ci.usage = buffer_usage;
    buffer_ci.sharingMode = SharingMode::E_EXCLUSIVE;
  }

  VmaAllocationCreateFlags f = 0;
  f |= VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
  f |= mapped ? VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_MAPPED_BIT : 0;

  auto allocation_information = GraphicsAllocator::Get()->AllocateBuffer(buffer_ci, buffer_, f);

  vma_allocation_ = allocation_information.first;
  mapped_memory_ = allocation_information.second;
}

Buffer::~Buffer() {
}

Buffer::Buffer(Buffer &&other) noexcept {
  buffer_ = std::exchange(other.buffer_, VK_NULL_HANDLE);
  vma_allocation_ = std::exchange(other.vma_allocation_, VK_NULL_HANDLE);
}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  std::swap(buffer_, other.buffer_);
  std::swap(vma_allocation_, other.vma_allocation_);
  std::swap(mapped_memory_, other.mapped_memory_);
  buffer_size_ = other.buffer_size_;
  return *this;
}

std::size_t Buffer::GetSize() const {
  return buffer_size_;
}

void Buffer::Map() {
  GraphicsAllocator::Get()->MapMemory(vma_allocation_, &mapped_memory_);
}

void Buffer::Unmap() {
  GraphicsAllocator::Get()->UnmapMemory(vma_allocation_);
  mapped_memory_ = nullptr;
}

VkBuffer Buffer::GetHandle() const {
  return buffer_;
}

std::span<std::byte> Buffer::GetMapped() {
  return std::span(mapped_memory_, buffer_size_);
}

} // namespace Yuggoth