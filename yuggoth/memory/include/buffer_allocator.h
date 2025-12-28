#ifndef YUGGOTH_BUFFER_ALLOCATOR_H
#define YUGGOTH_BUFFER_ALLOCATOR_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/core/allocators/virtual_allocator.h"
#include "common_types.h"
#include <memory>

namespace Yuggoth {

struct BufferAllocatorSpecification {
  uint64_t buffer_size_;
  uint64_t buffer_stride_;
  uint64_t buffer_typeid_;
  BufferUsageMask buffer_usage_;
  AllocationCreateMask allocation_mask_;
};

class BufferAllocator {
public:
  BufferAllocator(const BufferAllocatorSpecification &specification);

  Buffer *buffer();
  VirtualAllocator *allocator();

  BufferRange allocate(uint32_t count, uint32_t alignment);

  void free(uint32_t offset);

private:
  uint64_t stride_;
  uint64_t typeid_;
  std::unique_ptr<Buffer> buffer_;
  std::unique_ptr<VirtualAllocator> allocator_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_ALLOCATOR_H