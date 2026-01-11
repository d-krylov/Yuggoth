#ifndef YUGGOTH_VIRTUAL_ALLOCATOR_H
#define YUGGOTH_VIRTUAL_ALLOCATOR_H

#include "yuggoth/graphics/core/vma_allocator_types.h"
#include <unordered_map>
#include <vector>

namespace Yuggoth {

struct MemoryBlock {
  uint32_t offset_;
  uint32_t size_;
};

class VirtualAllocator {
public:
  VirtualAllocator() = default;

  VirtualAllocator(std::size_t size);

  ~VirtualAllocator();

  VirtualAllocator(const VirtualAllocator &) = delete;
  VirtualAllocator &operator=(const VirtualAllocator &) = delete;

  VirtualAllocator(VirtualAllocator &&other) noexcept;
  VirtualAllocator &operator=(VirtualAllocator &&other) noexcept;

  void Destroy();

  std::size_t Allocate(std::size_t size, std::size_t alignment, VirtualAllocationCreateMask mask);

  std::size_t GetSize() const;

  std::vector<MemoryBlock> GetAllocatorMap() const;

  void Free(std::size_t offset);

  void Clear();

private:
  std::size_t size_{0};
  VmaVirtualBlock virtual_block_{VK_NULL_HANDLE};
  std::unordered_map<std::size_t, VmaVirtualAllocation> allocations_;
};

} // namespace Yuggoth

#endif // YUGGOTH_VIRTUAL_ALLOCATOR_H