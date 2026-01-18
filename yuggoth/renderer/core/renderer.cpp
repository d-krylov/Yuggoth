#include "renderer.h"
#include "yuggoth/renderer/shaders/shader_library.h"
#include "yuggoth/renderer/shaders/pipeline_library.h"
#include "yuggoth/scene/core/scene.h"
#include "yuggoth/scene/core/entity.h"

namespace Yuggoth {

Renderer::Renderer(const RendererContext &renderer_context)
  : renderer_context_(renderer_context), base_renderer_backend_(this), raytrace_renderer_backend_(this) {
  Create();
  CreateDescriptorSets();
}

void Renderer::Create() {
  auto image_ci = ImageCreateInformation::CreateTexture2D(100, 100, Walle::Format::E_R32G32B32A32_SFLOAT, 1);
  image_ci.usage_ |= ImageUsageMaskBits::E_STORAGE_BIT;
  raytrace_image_ = Image(image_ci, SamplerCreateInformation());
}

void Renderer::OnViewportResize(uint32_t width, uint32_t height) {
  raytrace_image_.Resize(width, height);
}

void Renderer::CreateDescriptorSets() {
  std::vector<DescriptorPoolSize> descriptor_pool_sizes{{DescriptorType::E_SAMPLED_IMAGE, 65536}};

  descriptor_pool_ = DescriptorPool(descriptor_pool_sizes, DescriptorPoolCreateMaskBits::E_UPDATE_AFTER_BIND_BIT, 1);
}

const RendererContext &Renderer::GetRendererContext() const {
  return renderer_context_;
}

const Image &Renderer::GetStorageImage() const {
  return raytrace_image_;
}

void Renderer::Begin(Scene *scene, CommandBuffer &command_buffer, Image &target_image, Image &depth_image) {

  std::array<RenderingAttachmentInfo, 1> color_ai;
  color_ai[0].imageView = target_image.GetImageView();
  color_ai[0].imageLayout = ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL;
  color_ai[0].loadOp = AttachmentLoadOp::E_CLEAR;
  color_ai[0].storeOp = AttachmentStoreOp::E_STORE;
  color_ai[0].clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};

  RenderingAttachmentInfo depth_ai;
  depth_ai.imageView = depth_image.GetImageView();
  depth_ai.imageLayout = ImageLayout::E_DEPTH_ATTACHMENT_OPTIMAL;
  depth_ai.loadOp = AttachmentLoadOp::E_CLEAR;
  depth_ai.storeOp = AttachmentStoreOp::E_STORE;
  depth_ai.clearValue.depthStencil = {1.0f, 0};

  auto extent = target_image.GetImageCreateInformation().extent_;
  command_buffer.CommandBeginRendering(Walle::Extent2D(extent.width, extent.height), color_ai, &depth_ai);
}

void Renderer::DrawRayTrace(Scene *scene, CommandBuffer &command_buffer) {
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

  auto extent = raytrace_image_.GetImageCreateInformation().extent_;
  auto camera = scene->GetCurrentCamera();
  auto aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
  camera->SetAspect(aspect);

  // raytrace_renderer_backend_.Draw(&command_buffer_, scene, extent.width, extent.height);

  std::array<RenderingAttachmentInfo, 1> rendering_ai = {};

  auto &square_pipeline = renderer_context_.pipeline_library_->GetPipeline("square");

  // raytrace_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);

  // rendering_ai[0].imageView = target_image_.GetImageView();
  rendering_ai[0].imageLayout = ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL;
  rendering_ai[0].loadOp = AttachmentLoadOp::E_CLEAR;
  rendering_ai[0].storeOp = AttachmentStoreOp::E_STORE;
  rendering_ai[0].clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};

  command_buffer.CommandBeginRendering(Walle::Extent2D(extent.width, extent.height), rendering_ai);
  command_buffer.CommandSetViewport(0.0f, extent.height, extent.width, -float(extent.height));
  command_buffer.CommandSetScissor(0, 0, extent.width, extent.height);
  command_buffer.CommandEnableDepthTest(false);
  command_buffer.CommandEnableDepthWrite(false);
  command_buffer.CommandBindPipeline(square_pipeline.GetHandle(), PipelineBindPoint::E_GRAPHICS);

  command_buffer.CommandPushDescriptorSet(std::array{raytrace_image_.GetDescriptor()}, square_pipeline.GetPipelineLayout(), 0, 0,
                                          DescriptorType::E_COMBINED_IMAGE_SAMPLER, PipelineBindPoint::E_GRAPHICS);

  command_buffer.CommandDraw(6, 1, 0, 0);
  command_buffer.CommandEndRendering();
}

void Renderer::Draw(Scene *scene, CommandBuffer &command_buffer, const Extent2D &extent) {
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

  auto camera = scene->GetCurrentCamera();
  auto aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
  camera->SetAspect(aspect);

  command_buffer.CommandSetViewport(0.0f, extent.height, extent.width, -float(extent.height));
  command_buffer.CommandSetScissor(0, 0, extent.width, extent.height);
  // base_renderer_backend_.DrawDirect(&command_buffer_, scene, camera, ObjectMode::TEXTURED);

  base_renderer_backend_.DrawIndirect(&command_buffer, scene, camera, ObjectMode::TEXTURED);
}

void Renderer::End() {
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