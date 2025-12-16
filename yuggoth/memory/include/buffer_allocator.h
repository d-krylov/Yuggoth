#ifndef YUGGOTH_BUFFER_ALLOCATOR_H
#define YUGGOTH_BUFFER_ALLOCATOR_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/core/allocators/virtual_allocator.h"

namespace Yuggoth {

class BufferAllocator {
public:
  Buffer *GetBuffer();

  void Allocate(uint32_t size, uint32_t alignment);

private:
  Buffer buffer_;
  VirtualAllocator allocator_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_ALLOCATOR_H