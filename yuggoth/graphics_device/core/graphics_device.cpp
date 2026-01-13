#include "graphics_device.h"
#include "yuggoth/graphics/presentation/swapchain.h"
#include "yuggoth/core/tools/core.h"
#include <print>
#include <cassert>
#include <set>

namespace Yuggoth {

GraphicsDevice::GraphicsDevice() {
  CreateFrames();
}

GraphicsDevice::~GraphicsDevice() {
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