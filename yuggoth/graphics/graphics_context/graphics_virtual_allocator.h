#ifndef YUGGOTH_GRAPHICS_VIRTUAL_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_VIRTUAL_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>
#include <unordered_map>
#include <cstdint>

namespace Yuggoth {

class GraphicsVirtualAllocator {
public:
  GraphicsVirtualAllocator() = default;

  GraphicsVirtualAllocator(std::size_t size);

  ~GraphicsVirtualAllocator();

  GraphicsVirtualAllocator(const GraphicsVirtualAllocator &) = delete;
  GraphicsVirtualAllocator &operator=(const GraphicsVirtualAllocator &) = delete;

  GraphicsVirtualAllocator(GraphicsVirtualAllocator &&other) noexcept;
  GraphicsVirtualAllocator &operator=(GraphicsVirtualAllocator &&other) noexcept;

  void Destroy();

  std::size_t Allocate(std::size_t size, std::size_t alignment);

  void Free(uint32_t offset);

  void Clear();

private:
  std::size_t size_{0};
  VmaVirtualBlock virtual_block_{VK_NULL_HANDLE};
  std::unordered_map<std::size_t, VmaVirtualAllocation> allocations_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_VIRTUAL_ALLOCATOR_H