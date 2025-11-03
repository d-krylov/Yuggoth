#ifndef YUGGOTH_STRUCTURE_TOOLS_H
#define YUGGOTH_STRUCTURE_TOOLS_H

#include "graphics_enums.h"

namespace Yuggoth {

VkImageSubresourceRange GetImageSubresourceRange(ImageAspectMask aspect, uint32_t base_level = 0, uint32_t level_count = 1);

VkImageMemoryBarrier2 GetImageMemoryBarrier(VkImage image, VkImageLayout from_layout, VkImageLayout to_layout,
                                            VkPipelineStageFlags2 source_stage, VkPipelineStageFlags2 destination_stage,
                                            VkAccessFlags2 source_access, VkAccessFlags2 destination_access,
                                            const VkImageSubresourceRange &subresource);

} // namespace Yuggoth

#endif // YUGGOTH_STRUCTURE_TOOLS_H