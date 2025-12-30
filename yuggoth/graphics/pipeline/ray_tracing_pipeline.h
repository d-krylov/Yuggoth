#ifndef YUGGOTH_RAY_TRACING_PIPELINE_H
#define YUGGOTH_RAY_TRACING_PIPELINE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class RayTracingPipeline {
public:
  RayTracingPipeline();

  ~RayTracingPipeline();

  RayTracingPipeline(const RayTracingPipeline &) = delete;
  RayTracingPipeline &operator=(const RayTracingPipeline &) = delete;

  RayTracingPipeline(RayTracingPipeline &&other) noexcept;
  RayTracingPipeline &operator=(RayTracingPipeline &&other) noexcept;

protected:
  void Create();

private:
  VkPipeline ray_tracing_pipeline_{VK_NULL_HANDLE};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_RAY_TRACING_PIPELINE_H