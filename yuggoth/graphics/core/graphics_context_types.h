#ifndef YUGGOTH_GRAPHICS_CONTEXT_TYPES_H
#define YUGGOTH_GRAPHICS_CONTEXT_TYPES_H

#include "walle/walle.h"
#include <vma/vk_mem_alloc.h>
#include <vector>
#include <span>

namespace Yuggoth {

struct BufferCreateInformation;

struct BufferAllocationInformation {
  Walle::MemoryPropertyMask memory_property_;
  VmaAllocation allocation_ = VK_NULL_HANDLE;
  VkBuffer buffer_handle_ = VK_NULL_HANDLE;
  uint8_t *mapped_memory_ = nullptr;
  uint32_t memory_type_ = 0;
  uint64_t memory_size_ = 0;
};

struct ImageAllocationInformation {
  VmaAllocation allocation_ = VK_NULL_HANDLE;
  VkImage image_handle_ = VK_NULL_HANDLE;
};

struct GraphicsAllocatorStatistics {
  int64_t allocated_buffers_count_ = 0;
  int64_t allocated_images_count_ = 0;
  int64_t allocated_buffer_memory_ = 0;
  int64_t allocated_image_memory_ = 0;
};

struct AllocationCreateInformation {
  Walle::AllocationCreateMask allocation_create_mask_;
  Walle::MemoryPropertyMask required_memory_property_;
  Walle::MemoryPropertyMask preferred_memory_property_;
};

struct PhysicalDeviceFeatures {
  Walle::PhysicalDeviceRayTracingPipelineFeaturesKHR physical_device_ray_tracing_pipeline_features_;
  Walle::PhysicalDeviceRayQueryFeaturesKHR physical_device_ray_query_features;
  Walle::PhysicalDeviceMeshShaderFeaturesEXT physical_device_mesh_shader_features_;
  Walle::PhysicalDeviceAccelerationStructureFeaturesKHR physical_device_acceleration_structure_features_;
  Walle::PhysicalDeviceDescriptorBufferFeaturesEXT physical_device_descriptor_buffer_features_;
  Walle::PhysicalDeviceVulkan14Features physical_device_features_14_;
  Walle::PhysicalDeviceVulkan13Features physical_device_features_13_;
  Walle::PhysicalDeviceVulkan12Features physical_device_features_12_;
  Walle::PhysicalDeviceVulkan11Features physical_device_features_11_;
  Walle::PhysicalDeviceFeatures2 physical_device_features_2_;
};

struct PhysicalDeviceProperties {
  Walle::PhysicalDeviceRayTracingPipelinePropertiesKHR physical_device_raytracing_pipeline_properties_;
  Walle::PhysicalDeviceProperties2 physical_device_properties_2;
  Walle::PhysicalDeviceMemoryProperties2 physical_device_memory_properties_;
};

struct GraphicsContextCreateInformation {
  bool enable_debug = false;
  Walle::DebugUtilsMessageTypeMaskEXT debug_message_type_;
  Walle::DebugUtilsMessageSeverityMaskEXT debug_message_severity_;
  Walle::PFN_DebugUtilsMessengerCallbackEXT debug_callback_;
  std::vector<const char *> required_extensions_;
};

enum class QueueType {
  GRAPHICS,
  COMPUTE,
  TRANSFER
};

inline constexpr auto QUEUE_COUNT = std::to_underlying(QueueType::TRANSFER) + 1;

struct QueueInformation {
  std::array<VkQueue, QUEUE_COUNT> queues_ = {};
  std::array<uint32_t, QUEUE_COUNT> queue_indices_ = {};
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_CONTEXT_TYPES_H
