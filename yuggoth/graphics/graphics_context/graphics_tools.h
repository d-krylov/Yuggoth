#ifndef YUGGOTH_GRAPHICS_TOOLS_H
#define YUGGOTH_GRAPHICS_TOOLS_H

#include "yuggoth/core/tools/type_tools.h"
#include "yuggoth/graphics/core/graphics_structures.h"
#include <source_location>
#include <vector>

namespace Yuggoth {

template <typename T, typename FUNCTION, typename... ARGUMENTS> inline auto Enumerate(FUNCTION &&enumerate_function, ARGUMENTS &&...arguments) {
  using A = std::remove_pointer_t<decltype(&enumerate_function)>;
  using R = FunctionTraits<A>::arguments_t;
  using V = std::tuple_element_t<std::tuple_size_v<R> - 1, R>;
  auto count{0u};
  std::forward<FUNCTION>(enumerate_function)(std::forward<ARGUMENTS>(arguments)..., &count, nullptr);
  std::vector<T> ret(count);
  std::forward<FUNCTION>(enumerate_function)(std::forward<ARGUMENTS>(arguments)..., &count, reinterpret_cast<V>(ret.data()));
  return ret;
}

void VK_CHECK(VkResult result, std::source_location = std::source_location::current());

bool EvaluatePhysicalDevice(const VkPhysicalDevice physical_device);
std::array<int32_t, 3> PickPhysicalDeviceQueues(const VkPhysicalDevice physical_device);
std::vector<const char *> GetRequiredDeviceExtensions();

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TOOLS_H