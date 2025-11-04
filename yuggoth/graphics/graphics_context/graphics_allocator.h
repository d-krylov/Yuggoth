#ifndef YUGGOTH_GRAPHICS_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>
#include "graphics_context.h"

namespace Yuggoth {

using AllocationInformation = std::pair<VmaAllocation, std::byte *>;

class GraphicsAllocator {
public:
  GraphicsAllocator();

  ~GraphicsAllocator();

  static GraphicsAllocator *Get();

  VmaAllocation AllocateImage(const ImageCreateInfo &image_ci, VkImage &image);
  AllocationInformation AllocateBuffer(const BufferCreateInfo &buffer_ci, VkBuffer &buffer, VmaAllocationCreateFlags flags);

  void DestroyImage(VkImage image, VmaAllocation vma_allocation);
  void DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation);

  void MapMemory(VmaAllocation allocation, std::byte **memory);
  void UnmapMemory(VmaAllocation allocation);

protected:
  void CreateAllocator();

private:
  VmaAllocator vma_allocator_{VK_NULL_HANDLE};

  static GraphicsAllocator *graphics_allocator_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_ALLOCATOR_H
