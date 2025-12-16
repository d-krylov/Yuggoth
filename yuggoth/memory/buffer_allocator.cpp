#include "yuggoth/memory/include/buffer_allocator.h"

namespace Yuggoth {

Buffer *BufferAllocator::GetBuffer() {
  return &buffer_;
}

void BufferAllocator::Allocate(uint32_t size, uint32_t alignment) {
  auto offset = allocator_.Allocate(size, alignment);
}

} // namespace Yuggoth