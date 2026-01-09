#ifndef YUGGOTH_GRAPHICS_PIPELINE_H
#define YUGGOTH_GRAPHICS_PIPELINE_H

#include "pipeline.h"

namespace Yuggoth {

struct GraphicsPipelineSpecification;

class GraphicsPipeline : public Pipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(const GraphicsPipelineSpecification &specification);

  Walle::PipelineBindPoint GetBindPoint() const override;

private:
  std::vector<VkDescriptorSetLayout> descriptor_set_layouts_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_PIPELINE_H