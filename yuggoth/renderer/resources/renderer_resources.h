#ifndef YUGGOTH_RENDERER_RESOURCES_H
#define YUGGOTH_RENDERER_RESOURCES_H

#include "frame.h"

namespace Yuggoth {

class RendererResources {
public:
  RendererResources();

  void BeginFrame();

  void EndFrame();

  Frame &GetCurrentFrame();

  void OnResize(uint32_t width, uint32_t height);

private:
  std::vector<Frame> frames_;
  uint64_t frame_index_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_RESOURCES_H