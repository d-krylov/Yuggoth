#ifndef YUGGOTH_GRAPHICS_ALLOCATOR_H
#define YUGGOTH_GRAPHICS_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>
#include "graphics_context.h"

namespace Yuggoth {

class GraphicsAllocator {
public:
  GraphicsAllocator();

  ~GraphicsAllocator();

  static GraphicsAllocator *Get();

  VmaAllocation AllocateImage(const VkImageCreateInfo &image_ci, VkImage &image);
  VmaAllocation AllocateBuffer(const VkBufferCreateInfo &buffer_ci, VkBuffer &buffer);

protected:
  void CreateAllocator();

private:
  VmaAllocator vma_allocator_{VK_NULL_HANDLE};

  static GraphicsAllocator *graphics_allocator_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_ALLOCATOR_H
