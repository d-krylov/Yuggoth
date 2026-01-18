#ifndef YUGGOTH_FRAME_H
#define YUGGOTH_FRAME_H

#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/synchronization/fence.h"
#include "yuggoth/graphics/synchronization/semaphore.h"
#include "yuggoth/graphics/image/image.h"

namespace Yuggoth {

struct BufferAndAllocation {
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VmaAllocation allocation_ = VK_NULL_HANDLE;
};

struct ImageAndAllocation {
  VkImage image_ = VK_NULL_HANDLE;
  VmaAllocation allocation_ = VK_NULL_HANDLE;
};

struct Frame {
  Frame();

  Frame(const Frame &) = delete;
  void operator=(const Frame &) = delete;

  Frame(Frame &&other) noexcept = default;
  Frame &operator=(Frame &&other) noexcept = default;

  void Begin();

  void End();

  void OnResize(uint32_t width, uint32_t height);

protected:
  void BeginDestroy();

public:
  CommandBuffer command_buffer_;
  Fence frame_fence_;
  Image target_image_;
  Image depth_image_;

  std::vector<BufferAndAllocation> destroyed_buffers_;
  std::vector<ImageAndAllocation> destroyed_images_;
};

} // namespace Yuggoth

#endif // YUGGOTH_FRAME_H