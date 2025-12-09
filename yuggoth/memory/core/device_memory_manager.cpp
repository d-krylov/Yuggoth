#include "device_memory_manager.h"

namespace Yuggoth {

DeviceMemoryManager::DeviceMemoryManager() {
  vkGetPhysicalDeviceMemoryProperties2(GraphicsContext::Get()->GetPhysicalDevice(), physical_device_memory_properties_);
}

const PhysicalDeviceMemoryProperties &DeviceMemoryManager::GetPhysicalDeviceMemoryProperties() const {
  return physical_device_memory_properties_.memoryProperties;
}

} // namespace Yuggoth