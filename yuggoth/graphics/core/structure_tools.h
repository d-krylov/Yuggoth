#ifndef YUGGOTH_STRUCTURE_TOOLS_H
#define YUGGOTH_STRUCTURE_TOOLS_H

#include "walle/walle.h"
#include <source_location>

namespace Yuggoth {

using namespace Walle;

Walle::ImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect = ImageAspectMaskBits::E_COLOR_BIT, uint32_t base_level = 0,
                                                      uint32_t level_count = 1, uint32_t base_layer = 0, uint32_t layer_count = 1);

Walle::PipelineColorBlendAttachmentState GetColorBlendAttachmentState(bool enable, BlendFactor from_color = BlendFactor::E_SRC_ALPHA,
                                                                      BlendFactor to_color = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                                      BlendOp color_operation = BlendOp::E_ADD, BlendFactor from_alpha = BlendFactor::E_ONE,
                                                                      BlendFactor to_alpha = BlendFactor::E_ONE_MINUS_SRC_ALPHA,
                                                                      BlendOp alpha_operation = BlendOp::E_ADD);

} // namespace Yuggoth

#endif // YUGGOTH_STRUCTURE_TOOLS_H