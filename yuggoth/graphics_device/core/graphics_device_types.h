#ifndef YUGGOTH_GRAPHICS_DEVICE_TYPES_H
#define YUGGOTH_GRAPHICS_DEVICE_TYPES_H

#include <memory>
#include <yuggoth/core/allocators/object_pool.h>

namespace Yuggoth {

class GraphicsDevice;
class CommandBuffer;
class Buffer;
class Image;

struct BufferDeleter {
  GraphicsDevice *graphics_device_;
  void operator()(Buffer *buffer) const noexcept;
};

struct ImageDeleter {
  GraphicsDevice *graphics_device_;
  void operator()(Image *image) const noexcept;
};

struct CommandBufferDeleter {
  GraphicsDevice *graphics_device_;
  void operator()(CommandBuffer *command_buffer) const noexcept;
};

using UniqueBufferHandle = std::unique_ptr<Buffer, BufferDeleter>;
using UniqueImageHandle = std::unique_ptr<Image, ImageDeleter>;
using UniqueCommandBufferHandle = std::unique_ptr<CommandBuffer, BufferDeleter>;

template <typename T> using VulkanObjectPool = ObjectPool<T>;

struct HandlePool {
  VulkanObjectPool<Buffer> buffers_;
  VulkanObjectPool<Image> images_;
  VulkanObjectPool<CommandBuffer> command_buffers_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_DEVICE_TYPES_H