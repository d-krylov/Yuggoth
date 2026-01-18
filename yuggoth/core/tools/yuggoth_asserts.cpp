#include "yuggoth_asserts.h"
#include <walle/walle.h>
#include <print>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location) {
  if (status == false) {
    std::println("ERROR: {0} in {1}:{2}", message, location.file_name(), location.line());
    std::abort();
  }
}

void VK_CHECK(VkResult result, std::source_location source_location) {
  auto status = Walle::Result(result);
  if (status != Walle::Result::E_SUCCESS) {
    std::println("VULKAN ERROR: {0} in {1}:{2}", Walle::ToString(status), source_location.file_name(), source_location.line());
    std::abort();
  }
}

} // namespace Yuggoth