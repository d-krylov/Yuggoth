#ifndef YUGGOTH_GRAPHICS_TOOLS_H
#define YUGGOTH_GRAPHICS_TOOLS_H

#include "yuggoth/core/tools/include/type_tools.h"
#include "yuggoth/graphics/core/graphics_types.h"
#include <source_location>
#include <vector>

namespace Yuggoth {

void VK_CHECK(VkResult result, std::source_location = std::source_location::current());

bool EvaluatePhysicalDevice(const VkPhysicalDevice physical_device);
std::array<int32_t, 3> PickPhysicalDeviceQueues(const VkPhysicalDevice physical_device);
std::vector<const char *> GetRequiredDeviceExtensions();

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TOOLS_H