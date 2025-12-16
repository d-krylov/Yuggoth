#ifndef YUGGOTH_GRAPHICS_CONTEXT_H
#define YUGGOTH_GRAPHICS_CONTEXT_H

#include "graphics_tools.h"
#include "yuggoth/graphics/core/graphics_types.h"
#include <string_view>
#include <array>

namespace Yuggoth {

class GraphicsContext {
public:
  GraphicsContext();
  ~GraphicsContext();

  const VkInstance GetInstance() const;
  const VkPhysicalDevice GetPhysicalDevice() const;
  const VkDevice GetDevice() const;

  int32_t GetGraphicsQueueIndex() const;
  VkQueue GetGraphicsQueue() const;

  static GraphicsContext *Get();

  static void SetObjectName(ObjectType object_type, std::string_view name, void *handle);

  const PhysicalDeviceProperties2 &GetPhysicalDeviceProperties() const;
  const PhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties() const;

protected:
  void CreateInstance();
  void PickPhysicalDevice();
  void CreateDevice();
  void SetPhysicalDeviceProperties();

private:
  VkInstance instance_{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT debug_messenger_{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkDevice device_{VK_NULL_HANDLE};
  std::array<VkQueue, 3> queues_{VK_NULL_HANDLE};
  std::array<int32_t, 3> queue_indices_{-1};
  PhysicalDeviceProperties2 physical_device_properties_;
  PhysicalDeviceMemoryProperties2 physical_device_memory_properties_;
  static GraphicsContext *graphics_context_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_CONTEXT_H