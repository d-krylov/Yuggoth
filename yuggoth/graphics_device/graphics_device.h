#ifndef YUGGOTH_GRAPHICS_DEVICE_H
#define YUGGOTH_GRAPHICS_DEVICE_H

#include "graphics_tools.h"
#include "yuggoth/graphics/core/graphics_types.h"
#include "yuggoth/graphics_device/include/buffer_allocator.h"
#include "graphics_device_types.h"
#include "frame.h"
#include "ankerl/unordered_dense.h"
#include <string_view>
#include <array>

namespace Yuggoth {

struct GraphicsDeviceSpecification {
  bool enable_debug = false;
  DebugUtilsMessageTypeMaskEXT debug_messgae_type;
  DebugUtilsMessageSeverityMaskEXT debug_message_severity;
};

class GraphicsDevice {
public:
  GraphicsDevice(const GraphicsDeviceSpecification &specification);

  ~GraphicsDevice();

  VkInstance GetInstance() const;
  VkPhysicalDevice GetPhysicalDevice() const;
  VkDevice GetDevice() const;

  int32_t GetGraphicsQueueIndex() const;
  VkQueue GetGraphicsQueue() const;

  static GraphicsDevice *Get();

  static void SetObjectName(ObjectType object_type, std::string_view name, void *handle);

  const PhysicalDeviceProperties2 &GetPhysicalDeviceProperties() const;
  const PhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties() const;
  const PhysicalDeviceRayTracingPipelinePropertiesKHR &GetPhysicalDeviceRayTracingPipelineProperties() const;

  // ALLOCATOR API
  AllocationInformation AllocateImage(const ImageCreateInfo &image_ci, VkImage &out_image);
  AllocationInformation AllocateBuffer(const BufferCreateInformation &buffer_ci, VkBuffer &out_buffer);

  void DestroyImage(VkImage image, VmaAllocation vma_allocation);
  void DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation);

  void MapMemory(VmaAllocation allocation, uint8_t **memory);
  void UnmapMemory(VmaAllocation allocation);

  void CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset);

  const GraphicsAllocatorStatistics &GetGraphicsAllocatorStatistics() const;

  // FRAME API
  void BeginFrame();
  void EndFrame();
  Frame &GetCurrentFrame();
  void OnResize(uint32_t width, uint32_t height);

protected:
  void CreateInstance(bool enable_debug, DebugUtilsMessageTypeMaskEXT debug_type, DebugUtilsMessageSeverityMaskEXT debug_severity);
  void PickPhysicalDevice();
  void CreateDevice();
  void SetPhysicalDeviceProperties();

  void CreateGraphicsAllocator();
  void CreateFrames();

private:
  VkInstance instance_{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT debug_messenger_{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkDevice device_{VK_NULL_HANDLE};
  std::array<VkQueue, 3> queues_{VK_NULL_HANDLE};
  std::array<int32_t, 3> queue_indices_{-1};
  PhysicalDeviceProperties2 physical_device_properties_;
  PhysicalDeviceMemoryProperties2 physical_device_memory_properties_;
  PhysicalDeviceRayTracingPipelinePropertiesKHR physical_device_raytracing_pipeline_properties_;

  // ALLOCATOR
  VmaAllocator allocator_{VK_NULL_HANDLE};
  GraphicsAllocatorStatistics graphics_allocator_statistics_;
  // GLOBAL BUFFERS
  ankerl::unordered_dense::map<std::size_t, BufferAllocator> global_device_allocators_;
  // FRAMES
  std::vector<Frame> frames_;

  uint64_t frame_index_ = 0;

  static GraphicsDevice *graphics_context_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_DEVICE_H