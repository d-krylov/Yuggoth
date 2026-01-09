#ifndef YUGGOTH_RAY_TRACING_PIPELINE_H
#define YUGGOTH_RAY_TRACING_PIPELINE_H

#include "pipeline.h"

namespace Yuggoth {

struct RayTracingPipelineSpecification;

class RayTracingPipeline : public Pipeline {
public:
  RayTracingPipeline() = default;

  RayTracingPipeline(const RayTracingPipelineSpecification &specification);

  Walle::PipelineBindPoint GetBindPoint() const override;

private:
  std::vector<VkDescriptorSetLayout> descriptor_set_layouts_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RAY_TRACING_PIPELINE_H