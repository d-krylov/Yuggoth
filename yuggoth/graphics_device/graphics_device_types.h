#ifndef YUGGOTH_GRAPHICS_DEVICE_TYPES_H
#define YUGGOTH_GRAPHICS_DEVICE_TYPES_H

#include "walle/walle.h"
#include <vma/vk_mem_alloc.h>
#include <span>

namespace Yuggoth {

struct BufferCreateInformation;

struct AllocationInformation {
  Walle::MemoryPropertyMask memory_property_;
  VmaAllocation allocation_ = nullptr;
  uint8_t *mapped_memory_ = nullptr;
  uint32_t memory_type_ = 0;
  uint64_t memory_size_ = 0;
};

struct GraphicsAllocatorStatistics {
  int64_t allocated_buffers_count_ = 0;
  int64_t allocated_images_count_ = 0;
  int64_t allocated_buffer_memory_ = 0;
  int64_t allocated_image_memory_ = 0;
};

struct PhysicalDeviceFeatures {
  Walle::PhysicalDeviceMeshShaderFeaturesEXT mesh_shader_features_;
  Walle::PhysicalDeviceAccelerationStructureFeaturesKHR physical_device_acceleration_structure_features_;
  Walle::PhysicalDeviceVulkan14Features physical_device_features_14_;
  Walle::PhysicalDeviceVulkan13Features physical_device_features_13_;
  Walle::PhysicalDeviceVulkan12Features physical_device_features_12_;
  Walle::PhysicalDeviceVulkan11Features physical_device_features_11_;
  Walle::PhysicalDeviceFeatures2 physical_device_features_2_;
};

struct PhysicalDeviceProperties {
  Walle::PhysicalDeviceRayTracingPipelinePropertiesKHR physical_device_raytracing_pipeline_properties_;
  Walle::PhysicalDeviceProperties2 physical_device_properties_2;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_DEVICE_TYPES_H
