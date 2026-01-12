#include "yuggoth/graphics_device/include/buffer_allocator.h"

namespace Yuggoth {

BufferRange BufferAllocator::allocate(uint32_t count, uint32_t alignment) {
  auto size = count * stride_;
  auto offset = allocator_->Allocate(size, alignment, VirtualAllocationCreateMaskBits::E_STRATEGY_MIN_OFFSET_BIT);
  BufferRange buffer_range_information;
  buffer_range_information.buffer_ = buffer_->GetHandle();
  buffer_range_information.offset_ = offset / stride_;
  buffer_range_information.count_ = count;
  buffer_range_information.stride_ = stride_;
  buffer_range_information.typeid_ = typeid_;
  return buffer_range_information;
}

BufferAllocator::BufferAllocator(const BufferAllocatorSpecification &specification)
  : stride_(specification.buffer_stride_), typeid_(specification.buffer_typeid_) {
  allocator_ = std::make_unique<VirtualAllocator>(specification.buffer_ci_.buffer_size_);
  buffer_ = std::make_unique<Buffer>(specification.buffer_ci_);
}

void BufferAllocator::free(uint32_t offset) {
  allocator_->Free(offset);
}

Buffer *BufferAllocator::buffer() {
  return buffer_.get();
}

VirtualAllocator *BufferAllocator::allocator() {
  return allocator_.get();
}

} // namespace Yuggoth