#include "yuggoth/core/tools/core.h"
#include "config/yuggoth_config.h"
#include <print>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location) {
  if (status == false) {
    std::println("ERROR: {0} in {1}:{2}", message, location.file_name(), location.line());
    std::abort();
  }
}

std::filesystem::path GetShadersRoot() {
  return YUGGOTH_PROJECT_BINARY_DIR / "spirv";
}

std::filesystem::path GetYuggothRoot() {
  return YUGGOTH_PROJECT_ROOT_DIR;
}

std::size_t AlignUp(std::size_t value, std::size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}

} // namespace Yuggoth