#ifndef YUGGOTH_GRAPHICS_CONTEXT_H
#define YUGGOTH_GRAPHICS_CONTEXT_H

#include "graphics_context_types.h"
#include <yuggoth/core/tools/yuggoth_macros.h>
#include <yuggoth/core/tools/yuggoth_asserts.h>
#include <volk/volk.h>

namespace Yuggoth {

class GraphicsContext {
public:
  DISABLE_COPY_SEMANTICS(GraphicsContext)
  DISABLE_MOVE_SEMANTICS(GraphicsContext)

  GraphicsContext(const GraphicsContextCreateInformation &graphics_context_ci);
  ~GraphicsContext();

  VkInstance GetInstance() const;
  VkPhysicalDevice GetPhysicalDevice() const;
  VkDevice GetDevice() const;

  int32_t GetGraphicsQueueIndex() const;
  VkQueue GetGraphicsQueue() const;

  int32_t GetTransferQueueIndex() const;
  VkQueue GetTransferQueue() const;

  const QueueInformation &GetQueueInformation(QueueType queue_type) const;

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
  void PickPhysicalDeviceQueues(std::vector<uint32_t> &queue_offsets);

private:
  VkInstance instance_{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT debug_messenger_{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkDevice device_{VK_NULL_HANDLE};
  PhysicalDeviceFeatures physical_device_features_;
  PhysicalDeviceProperties physical_device_properties_;
  std::array<QueueInformation, QUEUE_INDEX_COUNT> queue_information_;
  static GraphicsContext *graphics_context_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_CONTEXT_H