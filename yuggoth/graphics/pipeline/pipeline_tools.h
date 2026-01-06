
#ifndef YUGGOTH_PIPELINE_TOOLS_H
#define YUGGOTH_PIPELINE_TOOLS_H

#include "yuggoth/renderer/shaders/shader_module.h"

namespace Yuggoth {

VkPipelineLayout CreatePipelineLayout(std::span<const VkDescriptorSetLayout> set_layouts, std::span<const PushConstantRange> push_constants);
std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(std::span<const DescriptorSetSpecification> specifications);

} // namespace Yuggoth

#endif // YUGGOTH_PIPELINE_TOOLS_H
