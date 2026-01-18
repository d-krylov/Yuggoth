#ifndef YUGGOTH_GRAPHICS_DEVICE_H
#define YUGGOTH_GRAPHICS_DEVICE_H

#include "yuggoth/graphics/core/graphics_types.h"
#include <yuggoth/graphics_device/systems/resource_manager.h>
#include "yuggoth/graphics_device/systems/frame.h"
#include "graphics_device_types.h"
#include <string_view>
#include <array>

namespace Yuggoth {

class BufferCreateInformation;
class ImageCreateInformation;

class GraphicsDevice {
public:
  GraphicsDevice();

  ~GraphicsDevice();

  // FRAME API
  void BeginFrame();
  void EndFrame();

  Frame &GetCurrentFrame();
  HandlePool &GetHandlePool();
  ResourceManager &GetResourceManager();

  static GraphicsDevice *Get();

  // Resources
  UniqueBufferHandle CreateBuffer(const BufferCreateInformation &buffer_ci);
  UniqueImageHandle CreateImage(const ImageCreateInformation &image_ci);

  UniqueCommandBufferHandle RequestCommandBuffer();

  void OnResize(uint32_t width, uint32_t height);

protected:
  void CreateFrames();
  void SetResourceManager();

private:
  HandlePool handle_pool_;
  ResourceManager resource_manager_;
  std::vector<Frame> frames_;
  uint64_t frame_index_ = 0;

  static GraphicsDevice *graphics_device_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_DEVICE_H