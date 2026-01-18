#include "raytrace_renderer_backend.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include <yuggoth/graphics_device/core/graphics_device.h>
#include "yuggoth/scene/core/scene.h"
#include "renderer.h"

namespace Yuggoth {

RaytraceRendererBackend::RaytraceRendererBackend(Renderer *renderer) : renderer_(renderer) {
  auto renderer_context = renderer_->GetRendererContext();
  auto pipeline_library = renderer_context.pipeline_library_;
  auto &raytrace_pipeline = pipeline_library->GetRayTracingPipeline("ray");
  shader_binding_table_ = ShaderBindingTable(raytrace_pipeline.GetHandle(), 1, 1, 0);
}

void RaytraceRendererBackend::Draw(CommandBuffer *command_buffer, Scene *scene, uint32_t width, uint32_t height) {
  auto renderer_context = renderer_->GetRendererContext();
  auto pipeline_library = renderer_context.pipeline_library_;
  auto &raytrace_pipeline = pipeline_library->GetRayTracingPipeline("ray");
  auto &target_image = renderer_->GetStorageImage();

  auto graphics_device = renderer_context.graphics_device_;

  auto camera = scene->GetCurrentCamera();

  auto &vertex_buffer = graphics_device->GetResourceManager().GetStaticBufferAllocator<Vertex>().buffer_;
  auto &index_buffer = graphics_device->GetResourceManager().GetStaticBufferAllocator<uint32_t>().buffer_;

  command_buffer->CommandBindPipeline(raytrace_pipeline.GetHandle(), PipelineBindPoint::E_RAY_TRACING_KHR);

  command_buffer->CommandPushDescriptorSet(raytrace_pipeline.GetPipelineLayout(), 0, 0, renderer_->GetTopAccelerationStructure().GetHandle(),
                                           PipelineBindPoint::E_RAY_TRACING_KHR);

  command_buffer->CommandPushDescriptorSet(std::array{target_image.GetDescriptor()}, raytrace_pipeline.GetPipelineLayout(), 0, 1,
                                           DescriptorType::E_STORAGE_IMAGE, PipelineBindPoint::E_RAY_TRACING_KHR);

  command_buffer->CommandPushDescriptorSet(raytrace_pipeline, 0, 2, vertex_buffer.GetHandle(), DescriptorType::E_STORAGE_BUFFER);

  command_buffer->CommandPushDescriptorSet(raytrace_pipeline, 0, 3, index_buffer.GetHandle(), DescriptorType::E_STORAGE_BUFFER);

  command_buffer->CommandPushConstants(raytrace_pipeline.GetPipelineLayout(), ShaderStageMaskBits::E_RAYGEN_BIT_KHR, camera->GetPosition(), 0);

  command_buffer->CommandTraceRay(shader_binding_table_.GetRaygenRegion(), shader_binding_table_.GetMissRegion(), shader_binding_table_.GetHitRegion(),
                                  shader_binding_table_.GetCallableRegion(), width, height, 1);
}

} // namespace Yuggoth