#ifndef YUGGOTH_RESOURCE_MANAGER_H
#define YUGGOTH_RESOURCE_MANAGER_H

#include "resource_manager_types.h"

namespace Yuggoth {

class CommandBuffer;
class GraphicsDevice;

class ResourceManager {
public:
  ResourceManager(GraphicsDevice *graphics_device);

  template <typename T> StaticBufferAllocator &GetStaticBufferAllocator();
  template <typename T> void RegisterStaticBufferAllocator(const BufferCreateInformation &buffer_ci);

  template <typename T> StaticBufferRange UploadStaticBuffer(std::span<const T> data);

  void FreeRangeInStaticBuffer(const StaticBufferRange &static_buffer_range);

protected:
  std::size_t UploadStaticBuffer(std::size_t type_id, std::span<const std::byte> data);
  std::size_t UploadFromStagingBuffer(std::size_t type_id, std::span<const std::byte> data);

private:
  GraphicsDevice *graphics_device_;
  std::unordered_map<std::size_t, StaticBufferAllocator> static_buffers_;
};

} // namespace Yuggoth

#include "resource_manager.ipp"

#endif // YUGGOTH_RESOURCE_MANAGER_H