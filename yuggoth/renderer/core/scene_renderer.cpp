#include "scene_renderer.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include <print>

namespace Yuggoth {

struct PushConstants {
  Matrix4f projection_ = Matrix4f(1.0f);
  Matrix4f view_ = Matrix4f(1.0f);
  Matrix4f model_ = Matrix4f(1.0f);
};

SceneRenderer::SceneRenderer(const RendererContext &renderer_context) : renderer_context_(renderer_context) {
}

void SceneRenderer::DirectDraw(Scene *scene) {

  if (scene == nullptr) return;

  const auto &mesh_pipeline = renderer_context_.pipeline_library_->GetPipeline("mesh");

  command_buffer_.CommandBindPipeline(mesh_pipeline.GetHandle(), PipelineBindPoint::E_GRAPHICS);
  // command_buffer_.CommandBindPipeline(graphics_pipeline_.GetHandle(), PipelineBindPoint::E_GRAPHICS);
  command_buffer_.CommandEnableDepthTest(true);
  command_buffer_.CommandEnableDepthWrite(true);

  PushConstants push_constants;

  if (scene->GetCurrentCamera() != nullptr) {
    auto window_size = target_image_.GetSpecification().extent_;
    scene->GetCurrentCamera()->SetAspect(static_cast<float>(window_size.width) / static_cast<float>(window_size.height));
    push_constants.view_ = scene->GetCurrentCamera()->GetLookAt();
    push_constants.projection_ = scene->GetCurrentCamera()->GetProjection();
  }

  auto &registry = scene->GetRegistry();

  auto model_group = registry.group<ModelComponent>();

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model_group.get<ModelComponent>(model_entity);
    const auto &transform_component = model.GetComponent<Transform>();

    push_constants.model_ = transform_component.GetMatrix();
    command_buffer_.CommandPushConstants(mesh_pipeline.GetPipelineLayout(), ShaderStageMaskBits::E_MESH_BIT_EXT, push_constants, 0);

    if (!model_component.model_) continue;

    auto vbo = model_component.model_->GetVertexBufferRange();
    auto ibo = model_component.model_->GetIndexBufferRange();
    auto mbo = model_component.model_->GetMeshletBufferRange();

    command_buffer_.CommandPushDescriptorSet(mesh_pipeline.GetPipelineLayout(), 0, 0, vbo.buffer_, DescriptorType::E_STORAGE_BUFFER);
    command_buffer_.CommandPushDescriptorSet(mesh_pipeline.GetPipelineLayout(), 0, 1, ibo.buffer_, DescriptorType::E_STORAGE_BUFFER);
    command_buffer_.CommandPushDescriptorSet(mesh_pipeline.GetPipelineLayout(), 0, 2, mbo.buffer_, DescriptorType::E_STORAGE_BUFFER);

    // command_buffer_.CommandBindVertexBuffer(model_component.model_->GetVertexBuffer().GetHandle(), 0);
    // command_buffer_.CommandBindIndexBuffer(model_component.model_->GetIndexBuffer().GetHandle(), 0, IndexType::E_UINT32);

    auto meshlet_buffer_range = model_component.model_->GetMeshletBufferRange();

    command_buffer_.CommandDrawMeshTasks(meshlet_buffer_range.count_, 1, 1);

    // command_buffer_.CommandDrawIndexed(model_component.model_->GetIndexBuffer().GetSize() / sizeof(uint32_t), 1, 0, 0, 0);
  }
}

void SceneRenderer::IndirectDraw(Scene *scene) {
}

} // namespace Yuggoth