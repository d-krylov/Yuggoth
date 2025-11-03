#ifndef YUGGOTH_GRAPHICS_CONTEXT_H
#define YUGGOTH_GRAPHICS_CONTEXT_H

#include "graphics_tools.h"
#include "yuggoth/graphics/core/graphics_enums.h"
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

protected:
  void CreateInstance();
  void PickPhysicalDevice();
  void CreateDevice();

private:
  VkInstance instance_{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT debug_messenger_{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkDevice device_{VK_NULL_HANDLE};
  std::array<VkQueue, 3> queues_{VK_NULL_HANDLE};
  std::array<int32_t, 3> queue_indices_{-1};
  static GraphicsContext *graphics_context_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_CONTEXT_H