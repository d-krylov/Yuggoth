#ifndef YUGGOTH_GRAPHICS_CONTEXT_H
#define YUGGOTH_GRAPHICS_CONTEXT_H

#include "graphics_context_types.h"
#include <volk/volk.h>

namespace Yuggoth {

class GraphicsContext {
public:
  GraphicsContext(const GraphicsContextCreateInformation &graphics_context_ci);
  ~GraphicsContext();

  VkInstance GetInstance() const;
  VkPhysicalDevice GetPhysicalDevice() const;
  VkDevice GetDevice() const;

  int32_t GetGraphicsQueueIndex() const;
  VkQueue GetGraphicsQueue() const;

  const PhysicalDeviceFeatures &GetPhysicalDeviceFeatures() const;
  const PhysicalDeviceProperties &GetPhysicalDeviceProperties() const;

  void SetObjectName(Walle::ObjectType object_type, std::string_view name, void *handle);

  static GraphicsContext *Get();

protected:
  void CreateGraphicsContext(const GraphicsContextCreateInformation &graphics_context_ci);
  void CreateInstance(const GraphicsContextCreateInformation &graphics_context_ci);
  void PickPhysicalDevice();
  void CreateDevice();

  void SetPhysicalDeviceFeatures();

private:
  VkInstance instance_{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT debug_messenger_{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkDevice device_{VK_NULL_HANDLE};
  PhysicalDeviceFeatures physical_device_features_;
  PhysicalDeviceProperties physical_device_properties_;
  QueueInformation queue_information_;
  static GraphicsContext *graphics_context_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_CONTEXT_H