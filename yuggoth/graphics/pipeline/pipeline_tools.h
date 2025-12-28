
#ifndef YUGGOTH_PIPELINE_TOOLS_H
#define YUGGOTH_PIPELINE_TOOLS_H

#include "shader_module.h"

namespace Yuggoth {

VkPipelineLayout CreatePipelineLayout(std::span<const VkDescriptorSetLayout> set_layouts, std::span<const PushConstantRange> push_constants);
std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(std::span<const ShaderModule *const> shader_modules);

} // namespace Yuggoth

#endif // YUGGOTH_PIPELINE_TOOLS_H
