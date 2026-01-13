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

  VkPhysicalDeviceProperties device_properties{};
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);

  bool b = true;
  b &= (device_properties.apiVersion >= VK_API_VERSION_1_3);
  b &= (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

  return b;
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