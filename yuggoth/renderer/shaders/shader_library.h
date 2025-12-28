#ifndef YUGGOTH_SHADER_LIBRARY_H
#define YUGGOTH_SHADER_LIBRARY_H

#include <filesystem>
#include <unordered_map>
#include "yuggoth/graphics/pipeline/shader_module.h"

namespace Yuggoth {

class ShaderLibrary {
public:
  ShaderLibrary();

  const ShaderModule *GetShaderModule(const std::filesystem::path &relative_path) const;

protected:
  void ScanShaderDirectory();

private:
  std::unordered_map<std::filesystem::path, std::unique_ptr<ShaderModule>> shader_modules_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SHADER_LIBRARY_H