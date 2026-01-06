
#include "pipeline_tools.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include <unordered_map>
#include <string>

namespace Yuggoth {

VkPipelineLayout CreatePipelineLayout(std::span<const VkDescriptorSetLayout> set_layouts, std::span<const PushConstantRange> push_constants) {
  VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
  PipelineLayoutCreateInfo pipeline_layout_ci;
  pipeline_layout_ci.setLayoutCount = set_layouts.size();
  pipeline_layout_ci.pSetLayouts = set_layouts.data();
  pipeline_layout_ci.pushConstantRangeCount = push_constants.size();
  pipeline_layout_ci.pPushConstantRanges = push_constants.data();
  VK_CHECK(vkCreatePipelineLayout(GraphicsContext::Get()->GetDevice(), pipeline_layout_ci, nullptr, &pipeline_layout));
  return pipeline_layout;
}

VkDescriptorSetLayout CreateDescriptorSetLayout(std::span<const DescriptorSetLayoutBinding> descriptor_set_bindings,
                                                DescriptorSetLayoutCreateMask descriptor_set_create_mask) {

  auto binding_count = descriptor_set_bindings.size();
  std::vector<DescriptorBindingMask> binding_masks(binding_count);

  for (auto i = 0; i < binding_count; i++) {
    auto is_array = descriptor_set_bindings[i].descriptorCount > 1;
    auto is_runtime_array = descriptor_set_bindings[i].descriptorCount == 0;
    auto is_end = (i + 1 == binding_count);

    if (is_array || is_runtime_array) {
      binding_masks[i] |= DescriptorBindingMaskBits::E_PARTIALLY_BOUND_BIT;
    }

    if (descriptor_set_create_mask.HasBits(DescriptorSetLayoutCreateMaskBits::E_UPDATE_AFTER_BIND_POOL_BIT)) {
      binding_masks[i] |= DescriptorBindingMaskBits::E_UPDATE_AFTER_BIND_BIT;
      binding_masks[i] |= DescriptorBindingMaskBits::E_VARIABLE_DESCRIPTOR_COUNT_BIT;
    }
  }

  DescriptorSetLayoutBindingFlagsCreateInfo set_binding_flags_ci;
  set_binding_flags_ci.bindingCount = binding_masks.size();
  set_binding_flags_ci.pBindingFlags = binding_masks.data();

  DescriptorSetLayoutCreateInfo descriptor_set_layout_ci;
  descriptor_set_layout_ci.bindingCount = descriptor_set_bindings.size();
  descriptor_set_layout_ci.pBindings = descriptor_set_bindings.data();
  descriptor_set_layout_ci.flags = descriptor_set_create_mask;
  descriptor_set_layout_ci.pNext = &set_binding_flags_ci;

  VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
  VK_CHECK(vkCreateDescriptorSetLayout(GraphicsContext::Get()->GetDevice(), descriptor_set_layout_ci, 0, &descriptor_set_layout));
  return descriptor_set_layout;
}

std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(std::span<const DescriptorSetSpecification> specifications) {
  std::vector<VkDescriptorSetLayout> out;
  for (const auto &specification : specifications) {
    out.emplace_back(CreateDescriptorSetLayout(specification.descriptor_set_bindings_, specification.descriptor_set_create_mask_));
  }
  return out;
}

} // namespace Yuggoth
