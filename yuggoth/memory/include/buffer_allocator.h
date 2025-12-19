#ifndef YUGGOTH_BUFFER_ALLOCATOR_H
#define YUGGOTH_BUFFER_ALLOCATOR_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/core/allocators/virtual_allocator.h"

namespace Yuggoth {

struct BufferRangeInformation {
  uint32_t stride_; // element size
  uint32_t offset_; // base element index
  uint32_t count_;  // number of elements
};

class BufferAllocator {
public:
  BufferAllocator() = default;

  BufferAllocator(Buffer &&buffer, std::size_t element_size);

  Buffer *GetBuffer();
  BufferRangeInformation Allocate(uint32_t count, uint32_t alignment);

private:
  Buffer buffer_;
  VirtualAllocator allocator_;
  std::size_t element_size_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_ALLOCATOR_H