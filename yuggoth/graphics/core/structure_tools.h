#ifndef YUGGOTH_STRUCTURE_TOOLS_H
#define YUGGOTH_STRUCTURE_TOOLS_H

#include "yuggoth/graphics/wrappers/graphics_structures.h"

namespace Yuggoth {

ImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect = ImageAspectMaskBits::E_COLOR_BIT, uint32_t base_level = 0,
                                               uint32_t level_count = 1, uint32_t base_layer = 0, uint32_t layer_count = 1);

PipelineColorBlendAttachmentState GetColorBlendAttachmentState(bool enable, BlendFactor from_color = BlendFactor::E_SRC_ALPHA,
                                                               BlendFactor to_color = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                               BlendOp color_operation = BlendOp::E_ADD, BlendFactor from_alpha = BlendFactor::E_ONE,
                                                               BlendFactor to_alpha = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                               BlendOp alpha_operation = BlendOp::E_ADD);

RenderingAttachmentInfo GetRenderingAttachmentInfo(VkImageView view, ImageLayout layout, AttachmentLoadOp load, AttachmentStoreOp store);

} // namespace Yuggoth

#endif // YUGGOTH_STRUCTURE_TOOLS_H