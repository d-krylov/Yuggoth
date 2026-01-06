#include "graphics_tools.h"
#include <vulkan/vk_enum_string_helper.h>
#include <ranges>
#include <algorithm>
#include <print>

namespace Yuggoth {

void VK_CHECK(VkResult result, std::source_location source_location) {
  if (result != VK_SUCCESS) {
    std::println("VULKAN ERROR: {0} in {1}:{2}", string_VkResult(result), source_location.file_name(), source_location.line());
    std::abort();
  }
}

template <typename T> bool CompareStructures(const T &s1, const T &s2, uint32_t offset) {
  auto sa = std::as_bytes(std::span{&s1, 1});
  auto sb = std::as_bytes(std::span{&s2, 1});
  return std::ranges::equal(sa.subspan(offset), sb.subspan(offset));
}

bool CheckPhysicalDeviceFeatures(const VkPhysicalDevice physical_device) {
  PhysicalDeviceRayQueryFeaturesKHR physical_device_ray_query_features;
  PhysicalDeviceMeshShaderFeaturesEXT physical_device_mesh_shader_features;
  PhysicalDeviceVulkan13Features physical_device_features13;
  PhysicalDeviceVulkan12Features physical_device_features12;
  PhysicalDeviceFeatures2 physical_device_features2;
  physical_device_features2.pNext = &physical_device_features12;
  physical_device_features12.pNext = &physical_device_features13;
  physical_device_features13.pNext = &physical_device_mesh_shader_features;
  physical_device_mesh_shader_features.pNext = &physical_device_ray_query_features;

  vkGetPhysicalDeviceFeatures2(physical_device, physical_device_features2);

  bool status = true;
  status &= physical_device_ray_query_features.rayQuery;
  status &= physical_device_features13.synchronization2;
  status &= physical_device_features13.dynamicRendering;

  return status;
}

bool EvaluatePhysicalDevice(const VkPhysicalDevice physical_device) {

  VkPhysicalDeviceProperties device_properties{};
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);

  bool b = true;
  b &= (device_properties.apiVersion >= VK_API_VERSION_1_3);
  b &= (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

  if (b == true) {
    b &= CheckPhysicalDeviceFeatures(physical_device);
  }

  return b;
}

std::array<int32_t, 3> PickPhysicalDeviceQueues(const VkPhysicalDevice physical_device) {
  std::array<int32_t, 3> queue_indices = {-1, -1, -1};
  auto common_mask = QueueMaskBits::E_GRAPHICS_BIT | QueueMaskBits::E_COMPUTE_BIT;
  auto queue_properties = Enumerate<QueueFamilyProperties>(vkGetPhysicalDeviceQueueFamilyProperties, physical_device);
  auto common_queue = std::ranges::find_if(queue_properties, [&](const auto &p) { return p.queueFlags.HasBits(common_mask); });
  auto common_index = std::ranges::distance(queue_properties.begin(), common_queue);
  queue_indices.fill(common_index);
  return queue_indices;
}

std::vector<const char *> GetRequiredDeviceExtensions() {
  return {
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,        //
    VK_EXT_MESH_SHADER_EXTENSION_NAME,              //
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,     //
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,   //
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME, //
    VK_KHR_RAY_QUERY_EXTENSION_NAME,                //
    VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,          //
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,        //
    VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,   //
    VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME, //
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,      //
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,                //
    VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME         //
  };
}

} // namespace Yuggoth