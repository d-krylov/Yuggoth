
#include "pipeline.h"
#include "yuggoth/graphics_device/graphics_device.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include "yuggoth/graphics/descriptors/descriptor_set.h"
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
  VK_CHECK(vkCreatePipelineLayout(GraphicsDevice::Get()->GetDevice(), pipeline_layout_ci, nullptr, &pipeline_layout));
  return pipeline_layout;
}

std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(std::span<const DescriptorSetSpecification> specifications) {
  std::vector<VkDescriptorSetLayout> out;
  for (const auto &specification : specifications) {
    out.emplace_back(DescriptorSet::CreateDescriptorSetLayout(specification.descriptor_set_bindings_, specification.descriptor_set_create_mask_));
  }
  return out;
}

Pipeline::~Pipeline() {
}

Pipeline::Pipeline(Pipeline &&other) noexcept {
  pipeline_ = std::exchange(other.pipeline_, VK_NULL_HANDLE);
  pipeline_layout_ = std::exchange(other.pipeline_layout_, VK_NULL_HANDLE);
}

Pipeline &Pipeline::operator=(Pipeline &&other) noexcept {
  std::swap(pipeline_, other.pipeline_);
  std::swap(pipeline_layout_, other.pipeline_layout_);
  return *this;
}

VkPipeline Pipeline::GetHandle() const {
  return pipeline_;
}

VkPipelineLayout Pipeline::GetPipelineLayout() const {
  return pipeline_layout_;
}

} // namespace Yuggoth
