#include "shader_library.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/core/tools/include/filesystem.h"
#include <print>

namespace Yuggoth {

ShaderLibrary::ShaderLibrary() {
  ScanShaderDirectory();
}

void ShaderLibrary::ScanShaderDirectory() {
  auto root = FileSystem::GetYuggothShaderBinaryDirectory();
  auto prefix_size = root.string().size() + 1;
  for (const auto &entry : std::filesystem::recursive_directory_iterator(root)) {
    if (entry.is_regular_file()) {
      auto shader_path = entry.path();
      std::string_view path_string = shader_path.c_str();
      auto key = path_string.substr(prefix_size);
      shader_modules_.emplace(key, std::make_unique<ShaderModule>(shader_path));
    }
  }
}

const ShaderModule *ShaderLibrary::GetShaderModule(const std::filesystem::path &relative_path) const {
  return shader_modules_.at(relative_path).get();
}

const std::unordered_map<std::filesystem::path, std::unique_ptr<ShaderModule>> &ShaderLibrary::GetShaderModules() const {
  return shader_modules_;
}

} // namespace Yuggoth