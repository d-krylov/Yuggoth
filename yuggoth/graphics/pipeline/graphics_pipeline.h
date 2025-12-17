#ifndef YUGGOTH_GRAPHICS_PIPELINE_H
#define YUGGOTH_GRAPHICS_PIPELINE_H

#include "pipeline_tools.h"

namespace Yuggoth {

struct GraphicsPipelineSpecification {
  std::vector<std::filesystem::path> shader_paths_;
  std::vector<DynamicState> dynamic_states_{DynamicState::E_VIEWPORT, DynamicState::E_SCISSOR};
  std::vector<Format> color_formats_;
  Format depth_format_ = Format::E_UNDEFINED;
  Format stencil_format_ = Format::E_UNDEFINED;
  CullModeMask cull_mode_ = CullModeMaskBits::E_NONE;
};

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(const GraphicsPipelineSpecification &specification);
  ~GraphicsPipeline();

  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

  GraphicsPipeline(GraphicsPipeline &&other) noexcept;
  GraphicsPipeline &operator=(GraphicsPipeline &&other) noexcept;

  VkPipeline GetHandle() const;
  VkPipelineLayout GetPipelineLayout() const;

private:
  VkPipeline pipeline_{VK_NULL_HANDLE};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
  std::vector<VkDescriptorSetLayout> descriptor_set_layouts_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_PIPELINE_H