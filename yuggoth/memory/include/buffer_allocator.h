#ifndef YUGGOTH_BUFFER_ALLOCATOR_H
#define YUGGOTH_BUFFER_ALLOCATOR_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/core/allocators/virtual_allocator.h"
#include "yuggoth/core/tools/include/core_types.h"

namespace Yuggoth {

class BufferAllocator {
public:
  BufferAllocator() = default;

  BufferAllocator(Buffer &&buffer, std::size_t element_size);

  Buffer *GetBuffer();
  VirtualAllocator *GetAllocator();

  BufferRange Allocate(uint32_t count, uint32_t alignment);

  void Free(uint32_t offset);

private:
  Buffer buffer_;
  VirtualAllocator allocator_;
  std::size_t element_size_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_ALLOCATOR_H