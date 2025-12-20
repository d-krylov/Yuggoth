#include "yuggoth/memory/include/buffer_allocator.h"

namespace Yuggoth {

Buffer *BufferAllocator::GetBuffer() {
  return &buffer_;
}

VirtualAllocator *BufferAllocator::GetAllocator() {
  return &allocator_;
}

BufferRangeInformation BufferAllocator::Allocate(uint32_t count, uint32_t alignment) {
  auto size = count * element_size_;
  auto offset = allocator_.Allocate(size, alignment, VirtualAllocationCreateMaskBits::E_STRATEGY_MIN_OFFSET_BIT);
  BufferRangeInformation buffer_range_information;
  buffer_range_information.offset_ = offset / element_size_;
  buffer_range_information.count_ = count;
  buffer_range_information.stride_ = element_size_;
  return buffer_range_information;
}

void BufferAllocator::Free(uint32_t offset) {
  allocator_.Free(offset);
}

BufferAllocator::BufferAllocator(Buffer &&buffer, std::size_t element_size) : buffer_(std::move(buffer)), element_size_(element_size) {
  allocator_ = VirtualAllocator(buffer_.GetSize());
}

} // namespace Yuggoth