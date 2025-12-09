#ifndef YUGGOTH_DEVICE_MEMORY_MANAGER_H
#define YUGGOTH_DEVICE_MEMORY_MANAGER_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/graphics_context/graphics_virtual_allocator.h"

namespace Yuggoth {

class DeviceMemoryManager {
public:
  DeviceMemoryManager();

  template <typename T> void SetScratchData(std::span<const T> data);

  const PhysicalDeviceMemoryProperties &GetPhysicalDeviceMemoryProperties() const;

protected:
private:
  Buffer scratch_buffer_;
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  GraphicsVirtualAllocator scratch_allocator_;
  GraphicsVirtualAllocator vertex_allocator_;
  GraphicsVirtualAllocator index_allocator_;
  PhysicalDeviceMemoryProperties2 physical_device_memory_properties_;
};

} // namespace Yuggoth

#include "device_memory_manager.ipp"

#endif // YUGGOTH_DEVICE_MEMORY_MANAGER_H