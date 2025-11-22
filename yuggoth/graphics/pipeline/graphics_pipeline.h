#ifndef YUGGOTH_GRAPHICS_PIPELINE_H
#define YUGGOTH_GRAPHICS_PIPELINE_H

#include "shader_module.h"

namespace Yuggoth {

struct GraphicsPipelineSpecification {
  std::vector<std::filesystem::path> shader_paths_;
  std::vector<DynamicState> dynamic_states_{DynamicState::E_VIEWPORT, DynamicState::E_SCISSOR};
  std::vector<Format> color_formats_;
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

  VkPipeline GetPipeline() const;
  VkPipelineLayout GetPipelineLayout() const;

private:
  VkPipeline pipeline_{VK_NULL_HANDLE};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_PIPELINE_H