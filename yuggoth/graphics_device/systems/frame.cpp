#include "frame.h"
#include "yuggoth/graphics/core/graphics_context.h"

namespace Yuggoth {

Frame::Frame() {
  command_buffer_ = CommandBuffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  auto target_ci = ImageCreateInformation::CreateRenderTarget(100, 100, Walle::Format::E_R8G8B8A8_UNORM);
  auto depth_ci = ImageCreateInformation::CreateRenderTarget(100, 100, Walle::Format::E_D32_SFLOAT);
  target_ci.usage_ |= Walle::ImageUsageMaskBits::E_SAMPLED_BIT;
  target_image_ = Image(target_ci, SamplerCreateInformation());
  depth_image_ = Image(depth_ci, std::nullopt);
  frame_fence_ = Fence(Walle::FenceCreateMaskBits::E_SIGNALED_BIT);
}

void Frame::Begin() {
  frame_fence_.Wait();
  frame_fence_.Reset();
  command_buffer_.Reset();
  command_buffer_.Begin(Walle::CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  target_image_.SetImageLayout(Walle::ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, &command_buffer_);
}

void Frame::End() {
  command_buffer_.CommandEndRendering();
  target_image_.SetImageLayout(Walle::ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);
  command_buffer_.End();

  Walle::SubmitInfo submit;
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = command_buffer_.get();

  vkQueueSubmit(GraphicsContext::Get()->GetGraphicsQueue(), 1, submit, frame_fence_.GetHandle());
}

void Frame::OnResize(uint32_t width, uint32_t height) {

  auto current_extent = target_image_.GetImageCreateInformation().extent_;

  if (current_extent.width != width && current_extent.height != height) {
    frame_fence_.Wait();
    target_image_.Resize(width, height);
    depth_image_.Resize(width, height);
  }
}

} // namespace Yuggoth