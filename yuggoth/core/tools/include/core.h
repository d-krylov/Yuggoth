#ifndef YUGGOTH_CORE_H
#define YUGGOTH_CORE_H

#include "core_types.h"
#include <filesystem>
#include <source_location>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location = std::source_location::current());

std::filesystem::path GetShadersRoot();

std::filesystem::path GetYuggothRoot();

std::size_t AlignUp(std::size_t value, std::size_t alignment);

} // namespace Yuggoth

#endif // YUGGOTH_CORE_H