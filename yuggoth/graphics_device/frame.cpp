#include "frame.h"
#include "yuggoth/graphics_device/graphics_device.h"

namespace Yuggoth {

Frame::Frame() {
  command_buffer_ = CommandBuffer(GraphicsDevice::Get()->GetGraphicsQueueIndex());
  auto target_ci = ImageCreateInformation::CreateRenderTarget(100, 100, Walle::Format::E_R8G8B8A8_UNORM);
  auto depth_ci = ImageCreateInformation::CreateRenderTarget(100, 100, Walle::Format::E_D32_SFLOAT);
  target_ci.usage_ |= Walle::ImageUsageMaskBits::E_SAMPLED_BIT;
  target_image_ = Image(target_ci, SamplerSpecification());
  depth_image_ = Image(depth_ci, std::nullopt);
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

  vkQueueSubmit(GraphicsDevice::Get()->GetGraphicsQueue(), 1, submit, frame_fence_.GetHandle());
}

void Frame::OnResize(uint32_t width, uint32_t height) {

  auto current_extent = target_image_.GetImageCreateInformation().extent_;

  if (current_extent.width != width && current_extent.height != height) {
    frame_fence_.Wait();
    target_image_.Resize(width, height);
    depth_image_.Resize(width, height);
  }
}

uint32_t FRAMES_IN_FLIGHT = 3;
uint64_t current_frame_index_ = 0;

void GraphicsDevice::CreateFrames() {
  frames_.resize(FRAMES_IN_FLIGHT);
}

void GraphicsDevice::BeginFrame() {
  current_frame_index_ = frame_index_ % FRAMES_IN_FLIGHT;
  frames_[current_frame_index_].Begin();
}

void GraphicsDevice::EndFrame() {
  frames_[current_frame_index_].End();
  frame_index_++;
}

Frame &GraphicsDevice::GetCurrentFrame() {
  return frames_[current_frame_index_];
}

void GraphicsDevice::OnResize(uint32_t width, uint32_t height) {
  GetCurrentFrame().OnResize(width, height);
}

} // namespace Yuggoth