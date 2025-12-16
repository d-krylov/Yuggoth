#ifndef YUGGOTH_GRAPHICS_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_ALLOCATOR_H

#include "graphics_context.h"
#include <span>

namespace Yuggoth {

struct AllocationInformation {
  VmaAllocation allocation_{nullptr};
  std::byte *mapped_memory_{nullptr};
};

struct GraphicsAllocatorStatistics {
  std::size_t allocated_buffers_count_ = 0;
  std::size_t allocated_images_count_ = 0;
};

class GraphicsAllocator {
public:
  GraphicsAllocator();

  ~GraphicsAllocator();

  static GraphicsAllocator *Get();

  AllocationInformation AllocateImage(const ImageCreateInfo &image_ci, VkImage &out_image);
  AllocationInformation AllocateBuffer(const BufferCreateInfo &buffer_ci, VkBuffer &out_buffer, AllocationCreateMask allocation_mask);

  void DestroyImage(VkImage image, VmaAllocation vma_allocation);
  void DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation);

  void MapMemory(VmaAllocation allocation, std::byte **memory);
  void UnmapMemory(VmaAllocation allocation);

  void CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset);

protected:
  void CreateAllocator();

private:
  VmaAllocator allocator_{VK_NULL_HANDLE};
  GraphicsAllocatorStatistics statistics_;

  static GraphicsAllocator *graphics_allocator_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_ALLOCATOR_H
