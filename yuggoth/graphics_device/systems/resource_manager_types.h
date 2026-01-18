#ifndef YUGGOTH_RESOURCE_MANAGER_TYPES_H
#define YUGGOTH_RESOURCE_MANAGER_TYPES_H

#include <yuggoth/core/allocators/virtual_allocator.h>
#include <yuggoth/graphics/buffer/buffer.h>

namespace Yuggoth {

struct StaticBufferAllocator {
  uint64_t stride_ = 0;
  uint64_t typeid_ = 0;
  Buffer buffer_;
  VirtualAllocator allocator_;
};

struct StaticBufferRange {
  VkBuffer buffer_ = VK_NULL_HANDLE;
  uint64_t typeid_ = 0;
  uint32_t stride_ = 0; // element size
  uint32_t offset_ = 0; // base element index
  uint32_t count_ = 0;  // number of elements
};

} // namespace Yuggoth

#endif // YUGGOTH_RESOURCE_MANAGER_TYPES_H