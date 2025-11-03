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

} // namespace Yuggoth