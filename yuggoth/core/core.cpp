#include "yuggoth/core/include/core.h"
#include <print>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location) {
  if (status == false) {
    std::println("ERROR: {0} in {1}:{2}", message, location.file_name(), location.line());
    std::abort();
  }
}

} // namespace Yuggoth