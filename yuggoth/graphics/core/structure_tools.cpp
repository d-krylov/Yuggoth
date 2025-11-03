#include "structure_tools.h"

namespace Yuggoth {

VkImageMemoryBarrier2 GetImageMemoryBarrier(VkImage image, VkImageLayout from_layout, VkImageLayout to_layout,
                                            VkPipelineStageFlags2 source_stage, VkPipelineStageFlags2 destination_stage,
                                            VkAccessFlags2 source_access, VkAccessFlags2 destination_access,
                                            const VkImageSubresourceRange &subresource) {
  VkImageMemoryBarrier2 image_memory_barrier{};
  {
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    image_memory_barrier.srcStageMask = source_stage;
    image_memory_barrier.srcAccessMask = source_access;
    image_memory_barrier.dstStageMask = destination_stage;
    image_memory_barrier.dstAccessMask = destination_access;
    image_memory_barrier.oldLayout = from_layout;
    image_memory_barrier.newLayout = to_layout;
    image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.image = image;
    image_memory_barrier.subresourceRange = subresource;
  }
  return image_memory_barrier;
}

VkImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect, uint32_t base_level, uint32_t level_count) {
  VkImageSubresourceRange image_subresource_range{};
  {
    image_subresource_range.aspectMask = (uint32_t)aspect;
    image_subresource_range.baseMipLevel = base_level;
    image_subresource_range.levelCount = level_count;
    image_subresource_range.baseArrayLayer = 0;
    image_subresource_range.layerCount = VK_REMAINING_ARRAY_LAYERS;
  }
  return image_subresource_range;
}

} // namespace Yuggoth