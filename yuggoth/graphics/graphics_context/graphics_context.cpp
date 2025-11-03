#include "graphics_context.h"
#include "yuggoth/graphics/presentation/swapchain.h"
#include "yuggoth/core/include/core.h"
#include <print>
#include <cassert>
#include <set>

namespace Yuggoth {

GraphicsContext *GraphicsContext::graphics_context_instance_ = nullptr;

GraphicsContext *GraphicsContext::Get() {
  assert(graphics_context_instance_ != nullptr);
  return graphics_context_instance_;
}

const VkInstance GraphicsContext::GetInstance() const {
  assert(instance_ != VK_NULL_HANDLE);
  return instance_;
}

const VkPhysicalDevice GraphicsContext::GetPhysicalDevice() const {
  assert(physical_device_ != VK_NULL_HANDLE);
  return physical_device_;
}

const VkDevice GraphicsContext::GetDevice() const {
  assert(device_ != VK_NULL_HANDLE);
  return device_;
}

int32_t GraphicsContext::GetGraphicsQueueIndex() const {
  assert(queue_indices_[0] != -1);
  return queue_indices_[0];
}

VkQueue GraphicsContext::GetGraphicsQueue() const {
  assert(queues_[0] != VK_NULL_HANDLE);
  return queues_[0];
}

std::vector<const char *> GetInstanceLayers() {
  auto layers = Enumerate<VkLayerProperties>(vkEnumerateInstanceLayerProperties);

  std::vector required_layers = {"VK_LAYER_KHRONOS_validation"};

  return required_layers;
}

// clang-format off
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                    VkDebugUtilsMessageTypeFlagsEXT type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT *data,
                                                    void *user_data) {
  std::println("Validation Layer: {0}", data->pMessage);
  return VK_FALSE;
}
// clang-format on

void GraphicsContext::CreateInstance() {
  VK_CHECK(volkInitialize());

  VkApplicationInfo application_info{};
  {
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = "Yuggoth";
    application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info.pEngineName = "Yuggoth Engine";
    application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info.apiVersion = VK_API_VERSION_1_3;
  }

  std::vector<const char *> required_layers = GetInstanceLayers();
  std::vector<const char *> required_extensions{VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

  auto swapchain_extensions = Swapchain::GetSwapchainExtensions();
  required_extensions.insert(required_extensions.end(), swapchain_extensions.begin(), swapchain_extensions.end());

  auto severity = DebugUtilsMessageSeverityMaskBitsEXT::E_INFO_BIT_EXT | DebugUtilsMessageSeverityMaskBitsEXT::E_VERBOSE_BIT_EXT |
                  DebugUtilsMessageSeverityMaskBitsEXT::E_WARNING_BIT_EXT | DebugUtilsMessageSeverityMaskBitsEXT::E_ERROR_BIT_EXT;

  VkDebugUtilsMessengerCreateInfoEXT debug_ci{};
  {
    debug_ci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
    debug_ci.messageSeverity = (uint32_t)severity;
    debug_ci.pfnUserCallback = DebugCallback;
  }

  VkInstanceCreateInfo instance_ci{};
  {
    instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_ci.pApplicationInfo = &application_info;
    instance_ci.enabledLayerCount = required_layers.size();
    instance_ci.ppEnabledLayerNames = required_layers.data();
    instance_ci.enabledExtensionCount = required_extensions.size();
    instance_ci.ppEnabledExtensionNames = required_extensions.data();
  }

  instance_ci.pNext = &debug_ci;

  VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &instance_));

  volkLoadInstanceOnly(instance_);

  VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance_, &debug_ci, nullptr, &debug_messenger_));
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

  CORE_ASSERT(physical_device_ != VK_NULL_HANDLE, "No Physical Devices found");
}

void GraphicsContext::CreateDevice() {
  queue_indices_ = PickPhysicalDeviceQueues(physical_device_);

  std::array queue_priorities = {0.0f};
  std::vector<VkDeviceQueueCreateInfo> device_queue_cis;
  std::set<int32_t> unique_indices(queue_indices_.begin(), queue_indices_.end());

  for (const auto &queue_index : unique_indices) {
    VkDeviceQueueCreateInfo queue_ci{};
    {
      queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_ci.queueFamilyIndex = queue_index;
      queue_ci.pQueuePriorities = queue_priorities.data();
      queue_ci.queueCount = 1;
    }
    device_queue_cis.emplace_back(queue_ci);
  }

  VkPhysicalDeviceRayQueryFeaturesKHR ray_query_features{};
  {
    ray_query_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
    ray_query_features.rayQuery = true;
  }

  VkPhysicalDeviceMeshShaderFeaturesEXT mesh_shader_features{};
  {
    mesh_shader_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
    mesh_shader_features.meshShader = true;
    mesh_shader_features.taskShader = true;
    mesh_shader_features.meshShaderQueries = true;
    mesh_shader_features.pNext = nullptr;
  }

  VkPhysicalDeviceVulkan14Features physical_device_features_14{};
  {
    physical_device_features_14.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;
    physical_device_features_14.maintenance5 = true;
    physical_device_features_14.maintenance6 = true;
    physical_device_features_14.pushDescriptor = true;
  }

  VkPhysicalDeviceVulkan13Features physical_device_features_13{};
  {
    physical_device_features_13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    physical_device_features_13.synchronization2 = VK_TRUE;
    physical_device_features_13.dynamicRendering = VK_TRUE;
    physical_device_features_13.pNext = &physical_device_features_14;
  }

  VkPhysicalDeviceFeatures2 physical_device_features_2{};
  {
    physical_device_features_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physical_device_features_2.pNext = &physical_device_features_13;
  }

  auto required_device_extensions = GetRequiredDeviceExtensions();

  VkDeviceCreateInfo device_ci{};
  {
    device_ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_ci.pQueueCreateInfos = device_queue_cis.data();
    device_ci.queueCreateInfoCount = device_queue_cis.size();
    device_ci.ppEnabledExtensionNames = required_device_extensions.data();
    device_ci.enabledExtensionCount = required_device_extensions.size();
    device_ci.pNext = &physical_device_features_2;
  }

  VK_CHECK(vkCreateDevice(physical_device_, &device_ci, nullptr, &device_));

  volkLoadDevice(device_);

  vkGetDeviceQueue(GetDevice(), GetGraphicsQueueIndex(), 0, &queues_[0]);
}

GraphicsContext::GraphicsContext() {
  CreateInstance();
  PickPhysicalDevice();
  CreateDevice();

  graphics_context_instance_ = this;
}

GraphicsContext::~GraphicsContext() {
}

} // namespace Yuggoth