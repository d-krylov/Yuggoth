#include "buffer.h"

namespace Yuggoth {

// CONSTRUCTORS

Buffer::Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage) {
  VkBufferCreateInfo buffer_ci{};
  {
    buffer_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_ci.size = buffer_size;
    buffer_ci.usage = (uint32_t)buffer_usage;
    buffer_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  vma_allocation_ = GraphicsAllocator::Get()->AllocateBuffer(buffer_ci, buffer_);
}

Buffer::~Buffer() {
}

Buffer::Buffer(Buffer &&other) noexcept {
  buffer_ = std::exchange(other.buffer_, VK_NULL_HANDLE);
  vma_allocation_ = std::exchange(other.vma_allocation_, VK_NULL_HANDLE);
}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  return *this;
}

std::size_t Buffer::GetSize() const {
  return buffer_size_;
}

} // namespace Yuggoth