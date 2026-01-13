#ifndef YUGGOTH_GRAPHICS_DEVICE_H
#define YUGGOTH_GRAPHICS_DEVICE_H

#include "yuggoth/graphics/core/graphics_types.h"
#include "yuggoth/graphics_device/systems/frame.h"
#include <string_view>
#include <array>

namespace Yuggoth {

class GraphicsDevice {
public:
  GraphicsDevice();

  ~GraphicsDevice();

  // FRAME API
  void BeginFrame();
  void EndFrame();
  Frame &GetCurrentFrame();
  void OnResize(uint32_t width, uint32_t height);

protected:
  void CreateFrames();

private:
  std::vector<Frame> frames_;
  uint64_t frame_index_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_DEVICE_H