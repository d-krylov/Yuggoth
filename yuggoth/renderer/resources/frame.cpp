#include "frame.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

Frame::Frame() {
  command_buffer_ = CommandBuffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  target_image_ = Image2D(100, 100);
  depth_image_ = ImageDepth(100, 100);
  frame_fence_ = Fence(Walle::FenceCreateMaskBits::E_SIGNALED_BIT);
}

void Frame::Begin() {
  frame_fence_.Wait();
  frame_fence_.Reset();
  command_buffer_.Reset();
  command_buffer_.Begin(Walle::CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  target_image_.SetImageLayout(ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, &command_buffer_);
}

void Frame::End() {
  command_buffer_.CommandEndRendering();
  target_image_.SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer_);
  command_buffer_.End();

  Walle::SubmitInfo submit;
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = command_buffer_.get();

  vkQueueSubmit(GraphicsContext::Get()->GetGraphicsQueue(), 1, submit, frame_fence_.GetHandle());
}

void Frame::OnResize(uint32_t width, uint32_t height) {

  auto current_extent = target_image_.GetExtent();

  if (current_extent.width != width && current_extent.height != height) {
    frame_fence_.Wait();
    target_image_ = Image2D(width, height);
    depth_image_ = ImageDepth(width, height);
  }
}

} // namespace Yuggoth