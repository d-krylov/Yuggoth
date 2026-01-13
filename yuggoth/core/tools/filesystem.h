#ifndef YUGGOTH_FILESYSTEM_H
#define YUGGOTH_FILESYSTEM_H

#include <filesystem>
#include <vector>

namespace Yuggoth {

class FileSystem {
public:
  static std::filesystem::path GetYuggothRootDirectory();
  static std::filesystem::path GetYuggothShaderSourcesDirectory();
  static std::filesystem::path GetYuggothShaderBinaryDirectory();

  static std::vector<uint8_t> ReadBinaryFile(const std::filesystem::path &path);
};

} // namespace Yuggoth

#endif // YUGGOTH_FILESYSTEM_H