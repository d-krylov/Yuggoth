#include "yuggoth/core/tools/include/core.h"
#include "config/yuggoth_config.h"
#include <print>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location) {
  if (status == false) {
    std::println("ERROR: {0} in {1}:{2}", message, location.file_name(), location.line());
    std::abort();
  }
}

std::size_t AlignUp(std::size_t value, std::size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}

} // namespace Yuggoth