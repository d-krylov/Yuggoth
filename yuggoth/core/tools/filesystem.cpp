#include "yuggoth/core/tools/filesystem.h"
#include "yuggoth/core/tools/core.h"
#include "config/yuggoth_config.h"
#include <fstream>

namespace Yuggoth {

std::filesystem::path FileSystem::GetYuggothRootDirectory() {
  return YUGGOTH_PROJECT_ROOT_DIR;
}

std::filesystem::path FileSystem::GetYuggothShaderSourcesDirectory() {
  return YUGGOTH_PROJECT_ROOT_DIR / "assets" / "shaders";
}

std::filesystem::path FileSystem::GetYuggothShaderBinaryDirectory() {
  return YUGGOTH_PROJECT_BINARY_DIR / "spirv";
}

std::vector<uint8_t> FileSystem::ReadBinaryFile(const std::filesystem::path &path) {
  auto size = std::filesystem::file_size(path);
  CORE_ASSERT(std::filesystem::exists(path), "Invalid path");
  std::vector<uint8_t> buffer(size);
  std::ifstream input_file(path.string(), std::ios_base::binary);
  input_file.read(reinterpret_cast<char *>(buffer.data()), size);
  return buffer;
}

} // namespace Yuggoth