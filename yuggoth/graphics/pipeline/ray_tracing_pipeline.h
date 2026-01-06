#ifndef YUGGOTH_RAY_TRACING_PIPELINE_H
#define YUGGOTH_RAY_TRACING_PIPELINE_H

#include "pipeline_tools.h"

namespace Yuggoth {

struct RayTracingPipelineSpecification;

class RayTracingPipeline {
public:
  RayTracingPipeline() = default;

  RayTracingPipeline(const RayTracingPipelineSpecification &specification);

  ~RayTracingPipeline();

  VkPipeline GetHandle() const;
  VkPipelineLayout GetPipelineLayout() const;

  RayTracingPipeline(const RayTracingPipeline &) = delete;
  RayTracingPipeline &operator=(const RayTracingPipeline &) = delete;

  RayTracingPipeline(RayTracingPipeline &&other) noexcept;
  RayTracingPipeline &operator=(RayTracingPipeline &&other) noexcept;

private:
  VkPipeline ray_tracing_pipeline_{VK_NULL_HANDLE};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
  std::vector<VkDescriptorSetLayout> descriptor_set_layouts_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RAY_TRACING_PIPELINE_H