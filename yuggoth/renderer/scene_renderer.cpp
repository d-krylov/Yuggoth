#include "yuggoth/renderer/include/scene_renderer.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/core/tools/include/core.h"
#include <print>

namespace Yuggoth {

SceneRenderer::SceneRenderer() {
  Initialize();
  command_buffer_ = CommandBuffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
}

void SceneRenderer::Initialize() {
  target_image_ = Image2D(100, 100);
  GraphicsPipelineSpecification specification;
  specification.color_formats_.emplace_back(Format::E_R8G8B8A8_UNORM);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_TEST_ENABLE);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_WRITE_ENABLE);
  auto shaders = GetShadersRoot();
  specification.shader_paths_ = {shaders / "base" / "mesh.vert.spv", shaders / "base" / "mesh.frag.spv"};
  graphics_pipeline_ = GraphicsPipeline(specification);
}

const Image2D &SceneRenderer::GetImage() const {
  return target_image_;
}

void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
  target_image_ = Image2D(width, height);
  depth_image_ = ImageDepth(width, height);
}

void SceneRenderer::Draw() {
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

  auto &extent = target_image_.GetExtent();

  command_buffer_.CommandBeginRendering(Extent2D(extent.width, extent.height), color_ai, &depth_ai);
  command_buffer_.CommandBindPipeline(graphics_pipeline_.GetHandle(), PipelineBindPoint::E_GRAPHICS);
  command_buffer_.CommandSetViewport(0, 0, extent.width, extent.height);
  command_buffer_.CommandSetScissor(0, 0, extent.width, extent.height);
  command_buffer_.CommandEndRendering();

  target_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);
  command_buffer_.End();
  command_buffer_.Submit();
}

void SceneRenderer::Begin(Scene *scene) {

  auto &registry = scene->GetRegistry();

  auto model_group = registry.group<ModelComponent>();

  for (auto model_entity : model_group) {

    const auto &model_component = model_group.get<ModelComponent>(model_entity);

    if (!model_component.model_) continue;
  }
}

void SceneRenderer::End() {
}

} // namespace Yuggoth