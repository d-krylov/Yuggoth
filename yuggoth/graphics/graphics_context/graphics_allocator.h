#ifndef YUGGOTH_GRAPHICS_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_ALLOCATOR_H

#include "graphics_context.h"
#include <span>

namespace Yuggoth {

struct AllocationInformation {
  MemoryPropertyMask memory_property_;
  VmaAllocation allocation_ = nullptr;
  uint8_t *mapped_memory_ = nullptr;
  uint32_t memory_type_ = 0;
  uint64_t memory_size_ = 0;
};

struct GraphicsAllocatorStatistics {
  std::size_t allocated_buffers_count_ = 0;
  std::size_t allocated_images_count_ = 0;
  std::size_t allocated_buffer_memory_ = 0;
  std::size_t allocated_image_memory_ = 0;
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

  void MapMemory(VmaAllocation allocation, uint8_t **memory);
  void UnmapMemory(VmaAllocation allocation);

  void CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset);

  const GraphicsAllocatorStatistics &GetStatistics() const;

protected:
  void CreateAllocator();

private:
  VmaAllocator allocator_{VK_NULL_HANDLE};
  GraphicsAllocatorStatistics statistics_;

  static GraphicsAllocator *graphics_allocator_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_ALLOCATOR_H
