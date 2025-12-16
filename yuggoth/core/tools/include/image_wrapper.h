#ifndef YUGGOTH_IMAGE_WRAPPER_H
#define YUGGOTH_IMAGE_WRAPPER_H

#include <filesystem>

namespace Yuggoth {

class ImageWrapper {
public:
  ImageWrapper(const std::filesystem::path &path);

  ~ImageWrapper();

private:
  int32_t width_{0};
  int32_t height_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_WRAPPER_H