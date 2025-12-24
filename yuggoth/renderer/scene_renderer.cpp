#include "yuggoth/renderer/include/scene_renderer.h"
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

SceneRenderer::SceneRenderer(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
  Initialize();
  command_buffer_ = CommandBuffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
}

void SceneRenderer::Initialize() {
  target_image_ = Image2D(100, 100);
  depth_image_ = ImageDepth(100, 100);
  GraphicsPipelineSpecification specification;
  specification.color_formats_.emplace_back(Format::E_R8G8B8A8_UNORM);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_TEST_ENABLE);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_WRITE_ENABLE);
  specification.depth_format_ = Format::E_D32_SFLOAT;
  specification.cull_mode_ = CullModeMaskBits::E_BACK_BIT;
  auto shaders = GetShadersRoot();
  specification.shader_paths_ = {shaders / "base" / "mesh.vert.spv", shaders / "base" / "mesh.frag.spv"};
  graphics_pipeline_ = GraphicsPipeline(specification);
  specification.shader_paths_ = {shaders / "base" / "mesh_indirect.vert.spv", shaders / "base" / "mesh_indirect.frag.spv"};
  indirect_pipeline_ = GraphicsPipeline(specification);
  specification.shader_paths_ = {shaders / "mesh" / "mesh.mesh.spv", shaders / "mesh" / "mesh.frag.spv"};
  mesh_pipeline_ = GraphicsPipeline(specification);
  indirect_buffer_ = Buffer(2_MiB, BufferUsageMaskBits::E_INDIRECT_BUFFER_BIT, AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
  transform_buffer_ = Buffer(2_MiB, BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
}

const Image2D &SceneRenderer::GetImage() const {
  return target_image_;
}

void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
  target_image_ = Image2D(width, height);
  depth_image_ = ImageDepth(width, height);
}

void SceneRenderer::Draw(Scene *scene) {
  command_buffer_.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);

  target_image_.SetImageLayout(ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, &command_buffer_);

  std::array<RenderingAttachmentInfo, 1> color_ai;
  color_ai[0].imageView = target_image_.GetImageView();
  color_ai[0].imageLayout = ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL;
  color_ai[0].loadOp = AttachmentLoadOp::E_CLEAR;
  color_ai[0].storeOp = AttachmentStoreOp::E_STORE;
  color_ai[0].clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};

  RenderingAttachmentInfo depth_ai;
  depth_ai.imageView = depth_image_.GetImageView();
  depth_ai.imageLayout = ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL;
  depth_ai.loadOp = AttachmentLoadOp::E_CLEAR;
  depth_ai.storeOp = AttachmentStoreOp::E_STORE;
  depth_ai.clearValue.depthStencil = {1.0f, 0};

  auto &extent = target_image_.GetSpecification().extent_;

  command_buffer_.CommandBeginRendering(Extent2D(extent.width, extent.height), color_ai, &depth_ai);
  command_buffer_.CommandSetViewport(0.0f, extent.height, extent.width, -float(extent.height));
  command_buffer_.CommandSetScissor(0, 0, extent.width, extent.height);

  DirectDraw(scene);
  IndirectDraw(scene);

  command_buffer_.CommandEndRendering();

  target_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);
  command_buffer_.End();
  command_buffer_.Submit();
}

void SceneRenderer::Begin(Scene *scene) {
}

void SceneRenderer::DirectDraw(Scene *scene) {

  if (scene == nullptr) return;

  command_buffer_.CommandBindPipeline(graphics_pipeline_.GetHandle(), PipelineBindPoint::E_GRAPHICS);
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

  auto model_group = registry.group<ResourceOwningModelComponent>();

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model_group.get<ResourceOwningModelComponent>(model_entity);
    const auto &transform_component = model.GetComponent<Transform>();

    push_constants.model_ = transform_component.GetMatrix();
    command_buffer_.CommandPushConstants(graphics_pipeline_.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, push_constants, 0);

    if (!model_component.model_) continue;

    command_buffer_.CommandBindVertexBuffer(model_component.model_->GetVertexBuffer().GetHandle(), 0);
    command_buffer_.CommandBindIndexBuffer(model_component.model_->GetIndexBuffer().GetHandle(), 0, IndexType::E_UINT32);

    DrawResourceOwningModel(*model_component.model_);

    // command_buffer_.CommandDrawIndexed(model_component.model_->GetIndexBuffer().GetSize() / sizeof(uint32_t), 1, 0, 0, 0);
  }
}

void SceneRenderer::DrawResourceOwningModel(const ResourceOwningModel &model) {
  auto meshes = model.GetMeshes();
  auto images = model.GetImages();
  for (const auto mesh : meshes) {
    const auto &color_image = images[mesh.color_texture_index];
    command_buffer_.CommandPushDescriptorSet(graphics_pipeline_.GetPipelineLayout(), 0, 0, color_image.GetImageView(), color_image.GetSampler());
    command_buffer_.CommandDrawIndexed(mesh.indices_size, 1, mesh.indices_offset, 0, 0);
  }
}

void SceneRenderer::IndirectDraw(Scene *scene) {
  std::vector<DrawIndexedIndirectCommand> commands;
  std::vector<Matrix4f> transforms;

  if (scene == nullptr) return;

  command_buffer_.CommandBindPipeline(indirect_pipeline_.GetHandle(), PipelineBindPoint::E_GRAPHICS);
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

  command_buffer_.CommandBindIndexBuffer(buffer_manager_->GetIndexAllocator().GetBuffer()->GetHandle(), 0, IndexType::E_UINT32);
  command_buffer_.CommandPushDescriptorSet(indirect_pipeline_.GetPipelineLayout(), 0, 0,
                                           buffer_manager_->GetVertexAllocator().GetBuffer()->GetHandle());

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model_group.get<ModelComponent>(model_entity);
    const auto &transform_component = model.GetComponent<Transform>();
    transforms.emplace_back(transform_component.GetMatrix());

    command_buffer_.CommandPushConstants(indirect_pipeline_.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, push_constants, 0);

    if (!model_component.model_) continue;

    auto vertices = model_component.model_->GetVerticesInformation();
    auto indices = model_component.model_->GetIndicesInformation();

    DrawIndexedIndirectCommand command;
    command.firstIndex = indices.offset_;
    command.firstInstance = 0;
    command.indexCount = indices.count_;
    command.instanceCount = 1;
    command.vertexOffset = vertices.offset_;

    commands.emplace_back(command);
  }

  indirect_buffer_.SetData<DrawIndexedIndirectCommand>(commands);
  transform_buffer_.SetData<Matrix4f>(transforms);
  command_buffer_.CommandPushDescriptorSet(indirect_pipeline_.GetPipelineLayout(), 0, 1, transform_buffer_.GetHandle());

  command_buffer_.CommandDrawIndexedIndirect(indirect_buffer_.GetHandle(), 0, commands.size(), sizeof(DrawIndexedIndirectCommand));
}

void SceneRenderer::End() {
}

} // namespace Yuggoth