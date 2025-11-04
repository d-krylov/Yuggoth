#ifndef YUGGOTH_STRUCTURE_TOOLS_H
#define YUGGOTH_STRUCTURE_TOOLS_H

#include "graphics_structures.h"

namespace Yuggoth {

ImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect, uint32_t base_level = 0, uint32_t level_count = 1);

ImageMemoryBarrier2 GetImageMemoryBarrier(VkImage image, ImageLayout from_layout, ImageLayout to_layout, PipelineStageMask2 source_stage,
                                          PipelineStageMask2 destination_stage, AccessMask2 source_access, AccessMask2 destination_access,
                                          const ImageSubresourceRange &subresource);

PipelineColorBlendAttachmentState GetColorBlendAttachmentState(bool enable, BlendFactor from_color = BlendFactor::E_SRC_ALPHA,
                                                               BlendFactor to_color = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                               BlendOp color_operation = BlendOp::E_ADD,
                                                               BlendFactor from_alpha = BlendFactor::E_ONE,
                                                               BlendFactor to_alpha = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                               BlendOp alpha_operation = BlendOp::E_ADD);

} // namespace Yuggoth

#endif // YUGGOTH_STRUCTURE_TOOLS_H