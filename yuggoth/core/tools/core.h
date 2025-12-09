#ifndef YUGGOTH_CORE_H
#define YUGGOTH_CORE_H

#include <cstdint>
#include <filesystem>
#include <source_location>

namespace Yuggoth {

void CORE_ASSERT(bool status, std::string_view message, std::source_location location = std::source_location::current());

constexpr inline std::size_t operator""_KiB(unsigned long long int x) {
  return 1024ULL * x;
}

constexpr inline std::size_t operator""_MiB(unsigned long long int x) {
  return 1024_KiB * x;
}

std::filesystem::path GetShadersRoot();

std::filesystem::path GetYuggothRoot();

std::size_t AlignUp(std::size_t value, std::size_t alignment);

} // namespace Yuggoth

#endif // YUGGOTH_CORE_H