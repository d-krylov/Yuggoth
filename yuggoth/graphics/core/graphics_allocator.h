#ifndef YUGGOTH_GRAPHICS_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_ALLOCATOR_H

#include "graphics_context_types.h"
#include <yuggoth/core/tools/yuggoth_macros.h>
#include <span>

namespace Yuggoth {

struct BufferCreateInformation;

class GraphicsAllocator {
public:
  DISABLE_COPY_SEMANTICS(GraphicsAllocator)
  DISABLE_MOVE_SEMANTICS(GraphicsAllocator)

  GraphicsAllocator();

  ~GraphicsAllocator();

  static GraphicsAllocator *Get();

  ImageAllocationInformation AllocateImage(const Walle::ImageCreateInfo &image_ci, const AllocationCreateInformation &allocation_ci);
  BufferAllocationInformation AllocateBuffer(const Walle::BufferCreateInfo &buffer_ci, const AllocationCreateInformation &allocation_ci);

  void DestroyImage(VkImage image, VmaAllocation vma_allocation);
  void DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation);

  void MapMemory(VmaAllocation allocation, uint8_t **memory);
  void UnmapMemory(VmaAllocation allocation);

  void CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset);

  const GraphicsAllocatorStatistics &GetGraphicsAllocatorStatistics() const;

protected:
  void Create();

private:
  VmaAllocator graphics_allocator_{VK_NULL_HANDLE};
  GraphicsAllocatorStatistics graphics_allocator_statistics_;

  static GraphicsAllocator *graphics_allocator_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_ALLOCATOR_H