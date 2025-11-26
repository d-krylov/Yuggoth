#include "yuggoth/renderer/include/scene_renderer.h"
#include "yuggoth/core/include/core.h"

namespace Yuggoth {

SceneRenderer::SceneRenderer() {
  Initialize();
}

void SceneRenderer::Initialize() {
  vertex_buffer_ = Buffer(100_MiB, BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
  index_buffer_ = Buffer(100_MiB, BufferUsageMaskBits::E_INDEX_BUFFER_BIT, AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
  target_image_ = Image2D(100, 100);
}

const Image2D &SceneRenderer::GetImage() const {
  return target_image_;
}

void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
  auto extent = target_image_.GetExtent();
  if (width == extent.width && height == extent.height) return;
  target_image_ = Image2D(width, height);
}

void SceneRenderer::Draw(CommandBuffer &command_buffer) {

  target_image_.SetImageLayout(ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, &command_buffer);

  std::array<RenderingAttachmentInfo, 1> rendering_ai;
  {
    rendering_ai[0].imageView = target_image_.GetImageView();
    rendering_ai[0].imageLayout = ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL;
    rendering_ai[0].loadOp = AttachmentLoadOp::E_CLEAR;
    rendering_ai[0].storeOp = AttachmentStoreOp::E_STORE;
    rendering_ai[0].clearValue.color = {0.0f, 1.0f, 0.0f, 1.0f};
  }

  auto &extent = target_image_.GetExtent();

  command_buffer.CommandBeginRendering(Extent2D(extent.width, extent.height), rendering_ai);
  command_buffer.CommandSetViewport(0, 0, extent.width, extent.height);
  command_buffer.CommandSetScissor(0, 0, extent.width, extent.height);
  command_buffer.CommandEndRendering();

  target_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer);
}

} // namespace Yuggoth