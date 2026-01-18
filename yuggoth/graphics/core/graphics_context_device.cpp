#include "graphics_context.h"
#include "yuggoth/core/tools/type_tools.h"
#include <ranges>
#include <algorithm>
#include <print>

namespace Yuggoth {

void GraphicsContext::SetPhysicalDeviceFeatures() {
  auto &physical_device_ray_tracing_pipeline_features = physical_device_features_.physical_device_ray_tracing_pipeline_features_;
  physical_device_ray_tracing_pipeline_features.rayTracingPipeline = true;
  physical_device_ray_tracing_pipeline_features.rayTracingPipelineTraceRaysIndirect = true;
  physical_device_ray_tracing_pipeline_features.pNext = nullptr;

  auto &physical_device_ray_query_features = physical_device_features_.physical_device_ray_query_features;
  physical_device_ray_query_features.rayQuery = true;
  physical_device_ray_query_features.pNext = &physical_device_ray_tracing_pipeline_features;

  auto &physical_device_mesh_shader_features = physical_device_features_.physical_device_mesh_shader_features_;
  physical_device_mesh_shader_features.meshShader = true;
  physical_device_mesh_shader_features.taskShader = true;
  physical_device_mesh_shader_features.meshShaderQueries = true;
  physical_device_mesh_shader_features.pNext = &physical_device_ray_query_features;

  auto &physical_device_acceleration_structure_features = physical_device_features_.physical_device_acceleration_structure_features_;
  physical_device_acceleration_structure_features.accelerationStructure = true;
  physical_device_acceleration_structure_features.pNext = &physical_device_mesh_shader_features;

  auto &physical_device_descriptor_buffer_features = physical_device_features_.physical_device_descriptor_buffer_features_;
  physical_device_descriptor_buffer_features.descriptorBuffer = true;
  physical_device_descriptor_buffer_features.descriptorBufferCaptureReplay = true;
  physical_device_descriptor_buffer_features.descriptorBufferPushDescriptors = true;
  physical_device_descriptor_buffer_features.pNext = &physical_device_acceleration_structure_features;

  auto &physical_device_features_14 = physical_device_features_.physical_device_features_14_;
  physical_device_features_14.maintenance5 = true;
  physical_device_features_14.maintenance6 = true;
  physical_device_features_14.pushDescriptor = true;
  physical_device_features_14.pNext = &physical_device_descriptor_buffer_features;

  auto &physical_device_features_13 = physical_device_features_.physical_device_features_13_;
  physical_device_features_13.synchronization2 = true;
  physical_device_features_13.dynamicRendering = true;
  physical_device_features_13.maintenance4 = true;
  physical_device_features_13.pNext = &physical_device_features_14;

  auto &physical_device_features_12 = physical_device_features_.physical_device_features_12_;
  physical_device_features_12.bufferDeviceAddress = true;
  physical_device_features_12.descriptorIndexing = true;
  physical_device_features_12.shaderSampledImageArrayNonUniformIndexing = true;
  physical_device_features_12.descriptorBindingSampledImageUpdateAfterBind = true;
  physical_device_features_12.descriptorBindingUpdateUnusedWhilePending = true;
  physical_device_features_12.descriptorBindingPartiallyBound = true;
  physical_device_features_12.descriptorBindingVariableDescriptorCount = true;
  physical_device_features_12.runtimeDescriptorArray = true;
  physical_device_features_12.drawIndirectCount = true;
  physical_device_features_12.pNext = &physical_device_features_13;

  auto &physical_device_features_11 = physical_device_features_.physical_device_features_11_;
  physical_device_features_11.shaderDrawParameters = true;
  physical_device_features_11.pNext = &physical_device_features_12;

  auto &physical_device_features_2 = physical_device_features_.physical_device_features_2_;
  physical_device_features_2.features.multiDrawIndirect = true;
  physical_device_features_2.pNext = &physical_device_features_11;

  auto &physical_device_properties = physical_device_properties_.physical_device_properties_2;
  physical_device_properties.pNext = &physical_device_properties_.physical_device_raytracing_pipeline_properties_;

  vkGetPhysicalDeviceProperties2(GetPhysicalDevice(), physical_device_properties);
  vkGetPhysicalDeviceMemoryProperties2(GetPhysicalDevice(), physical_device_properties_.physical_device_memory_properties_);
}

bool EvaluatePhysicalDevice(const VkPhysicalDevice physical_device) {

  Walle::PhysicalDeviceProperties2 physical_device_properties;
  vkGetPhysicalDeviceProperties2(physical_device, physical_device_properties);

  bool b = true;
  b &= (physical_device_properties.properties.apiVersion >= VK_API_VERSION_1_3);
  b &= (physical_device_properties.properties.deviceType == Walle::PhysicalDeviceType::E_DISCRETE_GPU);

  return b;
}

bool FindQueue(std::span<const Walle::QueueFamilyProperties> properties, QueueType queue_type, std::span<QueueInformation> queue_inforamtion,
               std::span<uint32_t> queue_offsets, Walle::QueueMask required, Walle::QueueMask ignored) {
  auto type_index = std::to_underlying(queue_type);
  for (const auto &[family_index, queue_family_property] : std::views::enumerate(properties)) {
    if (queue_family_property.queueFlags.HasAnyBits(ignored)) continue;
    if (queue_family_property.queueFlags.HasBits(required)) {
      queue_inforamtion[type_index].family_index_ = family_index;
      queue_inforamtion[type_index].queue_mask_ = queue_family_property.queueFlags;
      queue_inforamtion[type_index].queue_index = queue_offsets[family_index]++;
      return true;
    }
  }
  return false;
}

void GraphicsContext::PickPhysicalDeviceQueues(std::vector<uint32_t> &queue_offsets) {
  auto properties = Enumerate<Walle::QueueFamilyProperties>(vkGetPhysicalDeviceQueueFamilyProperties, GetPhysicalDevice());
  queue_offsets.resize(properties.size(), 0);

  Walle::QueueMask GRAPHICS = Walle::QueueMaskBits::E_GRAPHICS_BIT;
  Walle::QueueMask COMPUTE = Walle::QueueMaskBits::E_COMPUTE_BIT;
  Walle::QueueMask TRANSFER = Walle::QueueMaskBits::E_TRANSFER_BIT;
  Walle::QueueMask GRAPHICS_AND_COMPUTE = GRAPHICS | COMPUTE;

  if (!FindQueue(properties, QueueType::GRAPHICS, queue_information_, queue_offsets, GRAPHICS_AND_COMPUTE, Walle::QueueMask(0))) {
  }

  if (!FindQueue(properties, QueueType::COMPUTE, queue_information_, queue_offsets, COMPUTE, GRAPHICS) &&
      !FindQueue(properties, QueueType::COMPUTE, queue_information_, queue_offsets, COMPUTE, Walle::QueueMask(0))) {
    queue_information_[QUEUE_COMPUTE_INDEX] = queue_information_[QUEUE_GRAPHICS_INDEX];
  }

  if (!FindQueue(properties, QueueType::TRANSFER, queue_information_, queue_offsets, TRANSFER, GRAPHICS_AND_COMPUTE) &&
      !FindQueue(properties, QueueType::TRANSFER, queue_information_, queue_offsets, COMPUTE, GRAPHICS)) {
    queue_information_[QUEUE_TRANSFER_INDEX] = queue_information_[QUEUE_COMPUTE_INDEX];
  }
}

void GraphicsContext::PickPhysicalDevice() {
  auto physical_devices = Enumerate<VkPhysicalDevice>(vkEnumeratePhysicalDevices, instance_);
  for (const auto physical_device : physical_devices) {
    auto b = EvaluatePhysicalDevice(physical_device);
    if (b == true) {
      physical_device_ = physical_device;
      break;
    }
  }
}

} // namespace Yuggoth