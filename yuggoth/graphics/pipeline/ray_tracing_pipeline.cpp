#include "ray_tracing_pipeline.h"

namespace Yuggoth {

RayTracingPipeline::RayTracingPipeline() {
}

RayTracingPipeline::~RayTracingPipeline() {
}

void RayTracingPipeline::Create() {

  VkRayTracingPipelineCreateInfoKHR ray_tracing_pipeline_ci{};
  {
    ray_tracing_pipeline_ci.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    ray_tracing_pipeline_ci.pNext = nullptr;
    // ray_tracing_pipeline_ci.flags =
    // ray_tracing_pipeline_ci.stageCount =
    // ray_tracing_pipeline_ci.pStages =
    // ray_tracing_pipeline_ci.groupCount =
    // ray_tracing_pipeline_ci.pGroups =
    // ray_tracing_pipeline_ci.maxPipelineRayRecursionDepth =
    // ray_tracing_pipeline_ci.pLibraryInfo =
    // ray_tracing_pipeline_ci.pLibraryInterface =
    // ray_tracing_pipeline_ci.pDynamicState =
    // ray_tracing_pipeline_ci.layout =
    // ray_tracing_pipeline_ci.basePipelineHandle =
    // ray_tracing_pipeline_ci.basePipelineIndex =
  }
}

RayTracingPipeline::RayTracingPipeline(RayTracingPipeline &&other) noexcept {
  ray_tracing_pipeline_ = std::exchange(other.ray_tracing_pipeline_, VK_NULL_HANDLE);
  pipeline_layout_ = std::exchange(other.pipeline_layout_, VK_NULL_HANDLE);
}

RayTracingPipeline &RayTracingPipeline::operator=(RayTracingPipeline &&other) noexcept {
  std::swap(ray_tracing_pipeline_, other.ray_tracing_pipeline_);
  std::swap(pipeline_layout_, other.pipeline_layout_);
  return *this;
}

} // namespace Yuggoth