#include "base_renderer_backend.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/graphics_device/core/graphics_device.h"
#include "yuggoth/asset/core/asset_manager.h"
#include "renderer.h"

namespace Yuggoth {

struct DrawIndirectContext {
  std::vector<Walle::DrawIndexedIndirectCommand> commands_;
  std::vector<Matrix4f> transforms_;
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
      Walle::DrawIndexedIndirectCommand command;
      command.firstIndex = indices.offset_ + mesh.indices_offset_;
      command.firstInstance = 0;
      command.indexCount = mesh.indices_count_;
      command.instanceCount = 1;
      command.vertexOffset = vertices.offset_;

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

  auto graphics_device = renderer_context.graphics_device_;
  auto &vbo = graphics_device->GetResourceManager().GetStaticBufferAllocator<Vertex>();
  auto &ibo = graphics_device->GetResourceManager().GetStaticBufferAllocator<uint32_t>();

  auto &transform = graphics_device->GetResourceManager().GetStaticBufferAllocator<Matrix4f>();

  command_buffer->CommandBindPipeline(pipeline.GetHandle(), PipelineBindPoint::E_GRAPHICS);
  command_buffer->CommandEnableDepthTest(true);
  command_buffer->CommandEnableDepthWrite(true);

  PushConstantMatrices push_constants;
  push_constants.projection_ = camera->GetProjection();
  push_constants.view_ = camera->GetLookAt();
  push_constants.transform_ = Matrix4f(1.0f);

  command_buffer->CommandPushConstants(pipeline.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, push_constants, 0);

  auto draw_indirect_context = CollectSceneEntities(scene, renderer_context);

  auto command_range = graphics_device->GetResourceManager().UploadStaticBuffer<Walle::DrawIndexedIndirectCommand>(draw_indirect_context.commands_);
  auto matrix_range = graphics_device->GetResourceManager().UploadStaticBuffer<Matrix4f>(draw_indirect_context.transforms_);
  auto light_range = graphics_device->GetResourceManager().UploadStaticBuffer<Light>(draw_indirect_context.lights_);

  command_buffer->CommandBindIndexBuffer(ibo.buffer_.GetHandle(), 0, IndexType::E_UINT32);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 0, vbo.buffer_.GetHandle(), DescriptorType::E_STORAGE_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 1, matrix_range.buffer_, DescriptorType::E_STORAGE_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline, 0, 3, light_range.buffer_, DescriptorType::E_UNIFORM_BUFFER);
  command_buffer->CommandPushDescriptorSet(pipeline.GetPipelineLayout(), 0, 4, renderer_->GetTopAccelerationStructure().GetHandle(),
                                           PipelineBindPoint::E_GRAPHICS);

  command_buffer->CommandDrawIndexedIndirect(command_range.buffer_, 0, draw_indirect_context.commands_.size(), sizeof(Walle::DrawIndexedIndirectCommand));

  graphics_device->GetResourceManager().FreeRangeInStaticBuffer(command_range);
  graphics_device->GetResourceManager().FreeRangeInStaticBuffer(matrix_range);
  graphics_device->GetResourceManager().FreeRangeInStaticBuffer(light_range);
}

} // namespace Yuggoth