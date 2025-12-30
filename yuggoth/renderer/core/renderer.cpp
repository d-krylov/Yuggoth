#include "renderer.h"
#include "yuggoth/renderer/shaders/shader_library.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"

namespace Yuggoth {

Renderer::Renderer(const RendererContext &renderer_context) : renderer_context_(renderer_context), base_renderer_backend_(this) {
  Create();
}

void Renderer::Create() {
  command_buffer_ = CommandBuffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  target_image_ = Image2D(100, 100);
  depth_image_ = ImageDepth(100, 100);
}

void Renderer::OnViewportResize(uint32_t width, uint32_t height) {
  target_image_ = Image2D(width, height);
  depth_image_ = ImageDepth(width, height);
}

void Renderer::CreateDescriptorSets() {
  std::vector<DescriptorPoolSize> descriptor_pool_sizes{{DescriptorType::E_SAMPLED_IMAGE, 1000}};
}

const RendererContext &Renderer::GetRendererConstext() const {
  return renderer_context_;
}

const Image2D &Renderer::GetImage() const {
  return target_image_;
}

void Renderer::Begin(Scene *scene) {
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

  auto extent = target_image_.GetExtent();
  command_buffer_.CommandBeginRendering(extent, color_ai, &depth_ai);
}

void Renderer::Draw(Scene *scene) {
  if (scene == nullptr) {
    return;
  }

  auto &registry = scene->GetRegistry();

  if (registry.view<ModelComponent>().empty()) {
    return;
  }

  if (registry.view<Camera>().empty()) {
    return;
  }

  BuildBottomAccelerationStructures(scene);
  BuildTopAccelerationStructure();

  auto extent = target_image_.GetExtent();
  auto camera = scene->GetCurrentCamera();
  auto aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
  camera->SetAspect(aspect);

  command_buffer_.CommandSetViewport(0.0f, extent.height, extent.width, -float(extent.height));
  command_buffer_.CommandSetScissor(0, 0, extent.width, extent.height);
  // base_renderer_backend_.DrawDirect(&command_buffer_, scene, camera, ObjectMode::TEXTURED);

  base_renderer_backend_.DrawIndirect(&command_buffer_, scene, camera, ObjectMode::TEXTURED);
}

void Renderer::End() {
  command_buffer_.CommandEndRendering();
  target_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);
  command_buffer_.End();
  command_buffer_.Submit();
}

void Renderer::BuildBottomAccelerationStructures(Scene *scene) {
  auto &registry = scene->GetRegistry();
  auto model_group = registry.group<ModelComponent>();

  bottom_transforms_.clear();

  for (auto model_entity : model_group) {

    auto model = Entity(model_entity, scene);

    const auto &model_component = model.GetComponent<ModelComponent>();
    const auto &transform_component = model.GetComponent<Transform>();

    if (!model_component.model_) continue;

    auto bottom_geometry = model_component.model_->GetBottomLevelGeometry();
    auto uuid = model_component.model_->uuid_;

    if (bottom_acceleration_structures_.contains(uuid) == false) {
      bottom_acceleration_structures_.emplace(uuid, bottom_geometry);
    }

    bottom_transforms_[uuid].emplace_back(transform_component.GetMatrix());
  }
}

void Renderer::BuildTopAccelerationStructure() {
  std::vector<BLASInstances> bottom_instances_data;
  for (const auto &bottom_acceleration_structure : bottom_acceleration_structures_) {
    BLASInstances bottom_instances;
    bottom_instances.acceleration_structure_ = bottom_acceleration_structure.second.GetHandle();
    bottom_instances.instances_ = bottom_transforms_[bottom_acceleration_structure.first];
    bottom_instances_data.emplace_back(bottom_instances);
  }
  top_acceleration_structure_ = AccelerationStructure(bottom_instances_data);
}

const AccelerationStructure &Renderer::GetTopAccelerationStructure() const {
  return top_acceleration_structure_;
}

} // namespace Yuggoth