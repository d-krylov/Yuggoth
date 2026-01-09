#include "base_renderer_backend.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/asset/core/asset_manager.h"
#include "renderer.h"

namespace Yuggoth {

struct DrawIndirectContext {
  std::vector<IndexedIndirectCommand> commands_;
  std::vector<TransformMatrix> transforms_;
  std::vector<Light> lights_;
};

DrawIndirectContext CollectSceneEntities(Scene *scene, const RendererContext &renderer_context) {
  DrawIndirectContext draw_indirect_context;

  auto asset_manager = renderer_context.asset_manager_;
  auto &asset_storage = asset_manager->GetAssetStorage();

  auto &registry = scene->GetRegistry();
  auto model_group = registry.group<ModelComponent>();

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model.GetComponent<ModelComponent>();

    if (!model_component.model_) continue;

    auto &model_loader = asset_storage.GetModelStorage(model_component.model_->uuid_);

    auto meshes = model_loader.GetMeshes();

    auto vertices = model_component.model_->GetVertexBufferRange();
    auto indices = model_component.model_->GetIndexBufferRange();

    const auto &transform_component = model.GetComponent<Transform>();

    for (const auto &mesh : meshes) {
      IndexedIndirectCommand command;
      command.first_index_ = indices.offset_ + mesh.indices_offset_;
      command.first_instance_ = 0;
      command.index_count_ = mesh.indices_count_;
      command.instance_count_ = 1;
      command.vertex_offset_ = vertices.offset_;

      draw_indirect_context.transforms_.emplace_back(transform_component.GetMatrix());
      draw_indirect_context.commands_.emplace_back(command);
    }

    /*
        IndexedIndirectCommand command;
        command.first_index_ = indices.offset_;
        command.first_instance_ = 0;
        command.index_count_ = indices.count_;
        command.instance_count_ = 1;
        command.vertex_offset_ = vertices.offset_;

        const auto &transform_component = model.GetComponent<Transform>();

        draw_indirect_context.transforms_.emplace_back(transform_component.GetMatrix());
        draw_indirect_context.commands_.emplace_back(command);
    */
  }

  auto light_group = registry.group<Light>();

  for (auto light_entity : light_group) {

    auto light = Entity(light_entity, scene);

    const auto &light_component = light.GetComponent<Light>();

    draw_indirect_context.lights_.emplace_back(light_component);
  }

  return draw_indirect_context;
}

BaseRendererBackend::BaseRendererBackend(Renderer *renderer) : renderer_(renderer) {
}

void BaseRendererBackend::DrawIndirect(CommandBuffer *command_buffer, Scene *scene, const Camera *camera, ObjectMode object_mode) {
  const auto &renderer_context = renderer_->GetRendererContext();
  const auto &pipeline = renderer_context.pipeline_library_->GetPipeline("base_indirect");

  auto buffer_manager = renderer_context.buffer_manager_;
  auto vbo = buffer_manager->GetBufferAllocator(Vertex::type_id).buffer();
  auto ibo = buffer_manager->GetBufferAllocator(Index32::type_id).buffer();
  auto transform = buffer_manager->GetBufferAllocator(TransformMatrix::type_id).buffer();

  command_buffer->CommandBindPipeline(pipeline.GetHandle(), PipelineBindPoint::E_GRAPHICS);
  command_buffer->CommandEnableDepthTest(true);
  command_buffer->CommandEnableDepthWrite(true);

  PushConstantMatrices push_constants;
  push_constants.projection_ = camera->GetProjection();
  push_constants.view_ = camera->GetLookAt();
  push_constants.transform_ = Matrix4f(1.0f);

  command_buffer->CommandPushConstants(pipeline.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, push_constants, 0);

  auto draw_indirect_context = CollectSceneEntities(scene, renderer_context);

  auto command_range = buffer_manager->GetBufferAllocator(IndexedIndirectCommand::type_id).allocate(draw_indirect_context.commands_.size(), 0);
  buffer_manager->UploadBuffer(command_buffer, command_range, std::as_bytes(std::span(draw_indirect_context.commands_)));

  auto matrix_range = buffer_manager->GetBufferAllocator(TransformMatrix::type_id).allocate(draw_indirect_context.transforms_.size(), 0);
  buffer_manager->UploadBuffer(command_buffer, matrix_range, std::as_bytes(std::span(draw_indirect_context.transforms_)));

  auto light_range = buffer_manager->GetBufferAllocator(Light::type_id).allocate(draw_indirect_context.lights_.size(), 0);
  buffer_manager->UploadBuffer(command_buffer, light_range, std::as_bytes(std::span(draw_indirect_context.lights_)));

  command_buffer->CommandBindIndexBuffer(ibo->GetHandle(), 0, IndexType::E_UINT32);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 0, vbo->GetHandle(), DescriptorType::E_STORAGE_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 1, matrix_range.buffer_, DescriptorType::E_STORAGE_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 3, light_range.buffer_, DescriptorType::E_UNIFORM_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline.GetPipelineLayout(), 0, 4, renderer_->GetTopAccelerationStructure().GetHandle(),
                                           PipelineBindPoint::E_GRAPHICS);

  command_buffer->CommandDrawIndexedIndirect(command_range.buffer_, 0, draw_indirect_context.commands_.size(), sizeof(IndexedIndirectCommand));

  buffer_manager->GetBufferAllocator(IndexedIndirectCommand::type_id).free(command_range.offset_ * command_range.stride_);
  buffer_manager->GetBufferAllocator(TransformMatrix::type_id).free(matrix_range.offset_ * matrix_range.stride_);
  buffer_manager->GetBufferAllocator(Light::type_id).free(light_range.offset_ * light_range.stride_);
}

} // namespace Yuggoth