#include "renderer_resources.h"

namespace Yuggoth {

uint32_t FRAMES_IN_FLIGHT = 2;
uint64_t current_frame_index_ = 0;

RendererResources::RendererResources() {
  frames_.resize(FRAMES_IN_FLIGHT);
}

void RendererResources::BeginFrame() {
  current_frame_index_ = frame_index_ % FRAMES_IN_FLIGHT;
  frames_[current_frame_index_].Begin();
}

void RendererResources::EndFrame() {
  frames_[current_frame_index_].End();
  frame_index_++;
}

Frame &RendererResources::GetCurrentFrame() {
  return frames_[current_frame_index_];
}

void RendererResources::OnResize(uint32_t width, uint32_t height) {
  GetCurrentFrame().OnResize(width, height);
}

} // namespace Yuggoth