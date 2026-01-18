#include "graphics_context.h"
#include "structure_tools.h"
#include "yuggoth/core/tools/type_tools.h"
#include <vector>
#include <cassert>

namespace Yuggoth {

GraphicsContext *GraphicsContext::graphics_context_instance_ = nullptr;

GraphicsContext *GraphicsContext::Get() {
  assert(graphics_context_instance_ != nullptr);
  return graphics_context_instance_;
}

// API
VkInstance GraphicsContext::GetInstance() const {
  assert(instance_ != VK_NULL_HANDLE);
  return instance_;
}

VkPhysicalDevice GraphicsContext::GetPhysicalDevice() const {
  assert(physical_device_ != VK_NULL_HANDLE);
  return physical_device_;
}

VkDevice GraphicsContext::GetDevice() const {
  assert(device_ != VK_NULL_HANDLE);
  return device_;
}

const PhysicalDeviceFeatures &GraphicsContext::GetPhysicalDeviceFeatures() const {
  return physical_device_features_;
}

const PhysicalDeviceProperties &GraphicsContext::GetPhysicalDeviceProperties() const {
  return physical_device_properties_;
}

int32_t GraphicsContext::GetGraphicsQueueIndex() const {
  assert(queue_information_[QUEUE_GRAPHICS_INDEX].family_index_ != -1);
  return queue_information_[QUEUE_GRAPHICS_INDEX].family_index_;
}

VkQueue GraphicsContext::GetGraphicsQueue() const {
  assert(queue_information_[QUEUE_GRAPHICS_INDEX].queue_ != VK_NULL_HANDLE);
  return queue_information_[QUEUE_GRAPHICS_INDEX].queue_;
}

int32_t GraphicsContext::GetTransferQueueIndex() const {
  assert(queue_information_[QUEUE_TRANSFER_INDEX].family_index_ != -1);
  return queue_information_[QUEUE_TRANSFER_INDEX].family_index_;
}

VkQueue GraphicsContext::GetTransferQueue() const {
  assert(queue_information_[QUEUE_TRANSFER_INDEX].queue_ != VK_NULL_HANDLE);
  return queue_information_[QUEUE_TRANSFER_INDEX].queue_;
}

const QueueInformation &GraphicsContext::GetQueueInformation(QueueType queue_type) const {
  auto queue_type_index = std::to_underlying(queue_type);
  return queue_information_[queue_type_index];
}

// PRIVATE
void GraphicsContext::CreateInstance(const GraphicsContextCreateInformation &graphics_context_ci) {
  VK_CHECK(volkInitialize());

  Walle::ApplicationInfo application_info;
  application_info.pApplicationName = "Yuggoth";
  application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.pEngineName = "Yuggoth Engine";
  application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.apiVersion = VK_API_VERSION_1_3;

  std::vector<const char *> required_layers;
  std::vector<const char *> required_extensions;

  if (graphics_context_ci.enable_debug) {
    required_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    required_layers.emplace_back("VK_LAYER_KHRONOS_validation");
  }

  auto &context_extensions = graphics_context_ci.required_extensions_;
  required_extensions.insert(required_extensions.end(), context_extensions.begin(), context_extensions.end());

  std::array enabled_validation{Walle::ValidationFeatureEnableEXT::E_SYNCHRONIZATION_VALIDATION_EXT, Walle::ValidationFeatureEnableEXT::E_BEST_PRACTICES_EXT};

  Walle::ValidationFeaturesEXT validation_features;
  validation_features.enabledValidationFeatureCount = enabled_validation.size();
  validation_features.pEnabledValidationFeatures = enabled_validation.data();

  Walle::DebugUtilsMessengerCreateInfoEXT debug_ci;
  debug_ci.messageType = graphics_context_ci.debug_message_type_;
  debug_ci.messageSeverity = graphics_context_ci.debug_message_severity_;
  debug_ci.pfnUserCallback = graphics_context_ci.debug_callback_;
  debug_ci.pNext = &validation_features;

  Walle::InstanceCreateInfo instance_ci;
  instance_ci.pApplicationInfo = &application_info;
  instance_ci.enabledLayerCount = required_layers.size();
  instance_ci.ppEnabledLayerNames = required_layers.data();
  instance_ci.enabledExtensionCount = required_extensions.size();
  instance_ci.ppEnabledExtensionNames = required_extensions.data();

  instance_ci.pNext = graphics_context_ci.enable_debug ? &debug_ci : nullptr;

  VK_CHECK(vkCreateInstance(instance_ci, nullptr, &instance_));

  volkLoadInstanceOnly(instance_);

  if (graphics_context_ci.enable_debug) {
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance_, debug_ci, nullptr, &debug_messenger_));
  }
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

void GraphicsContext::CreateDevice() {

  std::vector<uint32_t> queue_offsets;
  PickPhysicalDeviceQueues(queue_offsets);

  std::vector<DeviceQueueCreateInfo> device_queue_cis;

  std::array queue_priorities = {0.0f};
  for (auto family_index = 0; family_index < queue_offsets.size(); family_index++) {

    if (queue_offsets[family_index] == 0) continue;

    DeviceQueueCreateInfo queue_ci;
    queue_ci.queueFamilyIndex = family_index;
    queue_ci.pQueuePriorities = queue_priorities.data();
    queue_ci.queueCount = queue_offsets[family_index];

    device_queue_cis.emplace_back(queue_ci);
  }

  auto required_device_extensions = GetRequiredDeviceExtensions();

  Walle::DeviceCreateInfo device_ci;
  device_ci.pQueueCreateInfos = device_queue_cis.data();
  device_ci.queueCreateInfoCount = device_queue_cis.size();
  device_ci.ppEnabledExtensionNames = required_device_extensions.data();
  device_ci.enabledExtensionCount = required_device_extensions.size();
  device_ci.pNext = &physical_device_features_.physical_device_features_2_;

  VK_CHECK(vkCreateDevice(physical_device_, device_ci, nullptr, &device_));

  volkLoadDevice(device_);

  for (int i = 0; i < QUEUE_INDEX_COUNT; i++) {
    vkGetDeviceQueue(GetDevice(), queue_information_[i].family_index_, queue_information_[i].queue_index, &queue_information_[i].queue_);
  }
}

void GraphicsContext::CreateGraphicsContext(const GraphicsContextCreateInformation &graphics_context_ci) {
  CreateInstance(graphics_context_ci);
  PickPhysicalDevice();
  SetPhysicalDeviceFeatures();
  CreateDevice();
}

GraphicsContext::GraphicsContext(const GraphicsContextCreateInformation &graphics_context_ci) {
  CreateGraphicsContext(graphics_context_ci);

  graphics_context_instance_ = this;
}

GraphicsContext::~GraphicsContext() {
}

void GraphicsContext::SetObjectName(Walle::ObjectType object_type, std::string_view name, void *handle) {
  Walle::DebugUtilsObjectNameInfoEXT object_name_info;
  object_name_info.objectType = object_type;
  object_name_info.objectHandle = reinterpret_cast<uint64_t>(handle);
  object_name_info.pObjectName = name.data();
  vkSetDebugUtilsObjectNameEXT(GetDevice(), object_name_info);
}

} // namespace Yuggoth