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
  std::span<const VkDebugUtilsLabelEXT> command_buffer_labels(data->pCmdBufLabels, data->cmdBufLabelCount);
  for (const auto &command_buffer_label: command_buffer_labels) {
    std::println("Command Buffer Label: {}", command_buffer_label.pLabelName);
  }
  return false;
}
// clang-format on

void GraphicsContext::CreateInstance() {
  VK_CHECK(volkInitialize());

  ApplicationInfo application_info;
  {
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

  auto severity = DebugUtilsMessageSeverityMaskBitsEXT::E_ERROR_BIT_EXT | DebugUtilsMessageSeverityMaskBitsEXT::E_WARNING_BIT_EXT;

  std::array enabled_validation{ValidationFeatureEnableEXT::E_SYNCHRONIZATION_VALIDATION_EXT, ValidationFeatureEnableEXT::E_BEST_PRACTICES_EXT};

  ValidationFeaturesEXT validation_features;
  validation_features.enabledValidationFeatureCount = enabled_validation.size();
  validation_features.pEnabledValidationFeatures = enabled_validation.data();

  DebugUtilsMessengerCreateInfoEXT debug_ci{};
  {
    debug_ci.messageType = DebugUtilsMessageTypeMaskBitsEXT::E_VALIDATION_BIT_EXT;
    debug_ci.messageSeverity = severity;
    debug_ci.pfnUserCallback = DebugCallback;
    debug_ci.pNext = &validation_features;
  }

  InstanceCreateInfo instance_ci{};
  {
    instance_ci.pApplicationInfo = &application_info;
    instance_ci.enabledLayerCount = required_layers.size();
    instance_ci.ppEnabledLayerNames = required_layers.data();
    instance_ci.enabledExtensionCount = required_extensions.size();
    instance_ci.ppEnabledExtensionNames = required_extensions.data();
  }

  instance_ci.pNext = &debug_ci;

  VK_CHECK(vkCreateInstance(instance_ci, nullptr, &instance_));

  volkLoadInstanceOnly(instance_);

  VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance_, debug_ci, nullptr, &debug_messenger_));
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
  std::vector<DeviceQueueCreateInfo> device_queue_cis;
  std::set<int32_t> unique_indices(queue_indices_.begin(), queue_indices_.end());

  for (const auto &queue_index : unique_indices) {
    DeviceQueueCreateInfo queue_ci;
    {
      queue_ci.queueFamilyIndex = queue_index;
      queue_ci.pQueuePriorities = queue_priorities.data();
      queue_ci.queueCount = 1;
    }
    device_queue_cis.emplace_back(queue_ci);
  }

  PhysicalDeviceRayQueryFeaturesKHR ray_query_features{};
  ray_query_features.rayQuery = true;

  PhysicalDeviceMeshShaderFeaturesEXT mesh_shader_features;
  {
    mesh_shader_features.meshShader = true;
    mesh_shader_features.taskShader = true;
    mesh_shader_features.meshShaderQueries = true;
    mesh_shader_features.pNext = nullptr;
  }

  PhysicalDeviceVulkan14Features physical_device_features_14;
  {
    physical_device_features_14.maintenance5 = true;
    physical_device_features_14.maintenance6 = true;
    physical_device_features_14.pushDescriptor = true;
  }

  PhysicalDeviceVulkan13Features physical_device_features_13;
  {
    physical_device_features_13.synchronization2 = true;
    physical_device_features_13.dynamicRendering = true;
    physical_device_features_13.pNext = &physical_device_features_14;
  }

  PhysicalDeviceFeatures2 physical_device_features_2;
  physical_device_features_2.pNext = &physical_device_features_13;

  auto required_device_extensions = GetRequiredDeviceExtensions();

  DeviceCreateInfo device_ci;
  {
    device_ci.pQueueCreateInfos = device_queue_cis.data();
    device_ci.queueCreateInfoCount = device_queue_cis.size();
    device_ci.ppEnabledExtensionNames = required_device_extensions.data();
    device_ci.enabledExtensionCount = required_device_extensions.size();
    device_ci.pNext = &physical_device_features_2;
  }

  VK_CHECK(vkCreateDevice(physical_device_, device_ci, nullptr, &device_));

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

VkCommandBuffer GraphicsContext::CreateCommandBuffer(VkCommandPool command_pool, CommandBufferLevel level) {
  VkCommandBuffer command_buffer = VK_NULL_HANDLE;
  CommandBufferAllocateInfo command_buffer_ai;
  {
    command_buffer_ai.commandPool = command_pool;
    command_buffer_ai.level = level;
    command_buffer_ai.commandBufferCount = 1;
  }
  VK_CHECK(vkAllocateCommandBuffers(GetDevice(), command_buffer_ai, &command_buffer));
  return command_buffer;
}

VkCommandPool GraphicsContext::CreateCommandPool(uint32_t queue_family_index) {
  VkCommandPool command_pool = VK_NULL_HANDLE;
  CommandPoolCreateInfo command_pool_ci;
  {
    command_pool_ci.flags = CommandPoolCreateMaskBits::E_RESET_COMMAND_BUFFER_BIT;
    command_pool_ci.queueFamilyIndex = queue_family_index;
  }
  VK_CHECK(vkCreateCommandPool(GetDevice(), command_pool_ci, 0, &command_pool));
  return command_pool;
}

} // namespace Yuggoth