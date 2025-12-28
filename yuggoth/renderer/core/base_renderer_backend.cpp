#include "base_renderer_backend.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include "renderer.h"

namespace Yuggoth {

struct DrawIndirectContext {
  std::vector<IndexedIndirectCommand> commands_;
  std::vector<TransformMatrix> transforms_;
};

BaseRendererBackend::BaseRendererBackend(Renderer *renderer) : renderer_(renderer) {
}

void BaseRendererBackend::DrawDirect(CommandBuffer *command_buffer, Scene *scene, const Camera *camera, ObjectMode object_mode) {
  auto pipeline_name = (object_mode == ObjectMode::COLORED) ? "base_colored" : "base_textured";
  auto draw_function = (object_mode == ObjectMode::COLORED) ? &BaseRendererBackend::DrawDirectColored : &BaseRendererBackend::DrawDirectTextured;

  const auto &renderer_context = renderer_->GetRendererConstext();
  const auto &pipeline = renderer_context.pipeline_library_->GetPipeline(pipeline_name);

  command_buffer->CommandBindPipeline(pipeline.GetHandle(), PipelineBindPoint::E_GRAPHICS);

  command_buffer->CommandEnableDepthTest(true);
  command_buffer->CommandEnableDepthWrite(true);

  auto &registry = scene->GetRegistry();
  auto model_group = registry.group<ModelComponent>();

  PushConstantMatrices push_constants;

  for (auto entity : model_group) {
    auto model_entity = Entity(entity, scene);
    const auto &model_component = model_entity.GetComponent<ModelComponent>();
    const auto &transform_component = model_entity.GetComponent<Transform>();

    push_constants.projection_ = camera->GetProjection();
    push_constants.view_ = camera->GetLookAt();
    push_constants.transform_ = transform_component.GetMatrix();

    command_buffer->CommandPushConstants(pipeline.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, push_constants, 0);

    if (model_component.model_->GetAssetKind() == AssetKind::RESOURCE_OWNING_MODEL) {
      auto model = static_cast<const ResourceOwningModel *>(model_component.model_.get());
      auto vertices = model->GetVertexBufferRange();
      auto indices = model->GetIndexBufferRange();
      command_buffer->CommandBindVertexBuffer(vertices.buffer_, 0);
      command_buffer->CommandBindIndexBuffer(indices.buffer_, 0, IndexType::E_UINT32);
      (this->*draw_function)(command_buffer, &pipeline, model);
    }
  }
}

void BaseRendererBackend::DrawDirectColored(CommandBuffer *command_buffer, const GraphicsPipeline *pipeline, const ResourceOwningModel *model) {
  auto indices = model->GetIndexBufferRange();
  command_buffer->CommandDrawIndexed(indices.count_, 1, 0, 0, 0);
}

void BaseRendererBackend::DrawDirectTextured(CommandBuffer *command_buffer, const GraphicsPipeline *pipeline, const ResourceOwningModel *model) {
  auto meshes = model->GetMeshes();
  auto images = model->GetImages();
  for (const auto mesh : meshes) {
    const auto &color_image = images[mesh.color_texture_index];
    command_buffer->CommandPushDescriptorSet(pipeline->GetPipelineLayout(), 0, 0, color_image.GetImageView(), color_image.GetSampler());
    command_buffer->CommandDrawIndexed(mesh.indices_size, 1, mesh.indices_offset, 0, 0);
  }
}

DrawIndirectContext CollectIndexedIndirectCommands(Scene *scene) {
  DrawIndirectContext draw_indirect_context;

  auto &registry = scene->GetRegistry();
  auto model_group = registry.group<ModelComponent>();

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model_group.get<ModelComponent>(model_entity);

    if (!model_component.model_) continue;

    auto vertices = model_component.model_->GetVertexBufferRange();
    auto indices = model_component.model_->GetIndexBufferRange();

    IndexedIndirectCommand command;
    command.first_index_ = indices.offset_;
    command.first_instance_ = 0;
    command.index_count_ = indices.count_;
    command.instance_count_ = 1;
    command.vertex_offset_ = vertices.offset_;

    const auto &transform_component = model.GetComponent<Transform>();

    draw_indirect_context.transforms_.emplace_back(transform_component.GetMatrix());
    draw_indirect_context.commands_.emplace_back(command);
  }

  return draw_indirect_context;
}

void BaseRendererBackend::DrawIndirect(CommandBuffer *command_buffer, Scene *scene, const Camera *camera, ObjectMode object_mode) {
  const auto &renderer_context = renderer_->GetRendererConstext();
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

  auto draw_indirect_context = CollectIndexedIndirectCommands(scene);

  auto command_range = buffer_manager->GetBufferAllocator(IndexedIndirectCommand::type_id).allocate(draw_indirect_context.commands_.size(), 0);
  buffer_manager->UploadBuffer(command_buffer, command_range, std::as_bytes(std::span(draw_indirect_context.commands_)));

  auto matrix_range = buffer_manager->GetBufferAllocator(TransformMatrix::type_id).allocate(draw_indirect_context.transforms_.size(), 0);
  buffer_manager->UploadBuffer(command_buffer, matrix_range, std::as_bytes(std::span(draw_indirect_context.transforms_)));

  command_buffer->CommandBindIndexBuffer(ibo->GetHandle(), 0, IndexType::E_UINT32);
  command_buffer->CommandPushDescriptorSet(pipeline.GetPipelineLayout(), 0, 0, vbo->GetHandle());
  command_buffer->CommandPushDescriptorSet(pipeline.GetPipelineLayout(), 0, 1, transform->GetHandle());

  command_buffer->CommandDrawIndexedIndirect(command_range.buffer_, 0, draw_indirect_context.commands_.size(), sizeof(IndexedIndirectCommand));

  buffer_manager->GetBufferAllocator(IndexedIndirectCommand::type_id).free(command_range.offset_);
  buffer_manager->GetBufferAllocator(TransformMatrix::type_id).free(matrix_range.offset_);
}

} // namespace Yuggoth