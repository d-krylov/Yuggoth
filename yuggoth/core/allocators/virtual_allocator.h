#ifndef YUGGOTH_VIRTUAL_ALLOCATOR_H
#define YUGGOTH_VIRTUAL_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>
#include <unordered_map>
#include <cstdint>

namespace Yuggoth {

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

  std::size_t Allocate(std::size_t size, std::size_t alignment);

  void Free(std::size_t offset);

  void Clear();

private:
  std::size_t size_{0};
  VmaVirtualBlock virtual_block_{VK_NULL_HANDLE};
  std::unordered_map<std::size_t, VmaVirtualAllocation> allocations_;
};

} // namespace Yuggoth

#endif // YUGGOTH_VIRTUAL_ALLOCATOR_H