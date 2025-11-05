#include "structure_tools.h"

namespace Yuggoth {

ImageMemoryBarrier2 GetImageMemoryBarrier(VkImage image, ImageLayout from_layout, ImageLayout to_layout, PipelineStageMask2 source_stage,
                                          PipelineStageMask2 destination_stage, AccessMask2 source_access, AccessMask2 destination_access,
                                          const ImageSubresourceRange &subresource) {
  ImageMemoryBarrier2 image_memory_barrier;
  {
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

ImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect, uint32_t base_level, uint32_t level_count) {
  ImageSubresourceRange image_subresource_range;
  {
    image_subresource_range.aspectMask = aspect;
    image_subresource_range.baseMipLevel = base_level;
    image_subresource_range.levelCount = level_count;
    image_subresource_range.baseArrayLayer = 0;
    image_subresource_range.layerCount = 1;
  }
  return image_subresource_range;
}

PipelineColorBlendAttachmentState GetColorBlendAttachmentState(bool enable, BlendFactor from_color, BlendFactor to_color,
                                                               BlendOp color_operation, BlendFactor from_alpha, BlendFactor to_alpha,
                                                               BlendOp alpha_operation) {
  PipelineColorBlendAttachmentState color_blend_attachment_state;
  {
    color_blend_attachment_state.blendEnable = enable;
    color_blend_attachment_state.srcColorBlendFactor = from_color;
    color_blend_attachment_state.dstColorBlendFactor = to_color;
    color_blend_attachment_state.colorBlendOp = color_operation;
    color_blend_attachment_state.srcAlphaBlendFactor = from_alpha;
    color_blend_attachment_state.dstAlphaBlendFactor = to_alpha;
    color_blend_attachment_state.alphaBlendOp = alpha_operation;
    color_blend_attachment_state.colorWriteMask =
      ColorComponentMaskBits::E_R_BIT | ColorComponentMaskBits::E_G_BIT | ColorComponentMaskBits::E_B_BIT | ColorComponentMaskBits::E_A_BIT;
  }
  return color_blend_attachment_state;
}

} // namespace Yuggoth