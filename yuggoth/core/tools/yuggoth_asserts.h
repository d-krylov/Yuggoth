#ifndef YUGGOTH_ASSERTS_H
#define YUGGOTH_ASSERTS_H

#include <source_location>
#include <string_view>
#include <vulkan/vulkan_core.h>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location = std::source_location::current());

void VK_CHECK(VkResult result, std::source_location source_location = std::source_location::current());

} // namespace Yuggoth

#endif // YUGGOTH_ASSERTS_H