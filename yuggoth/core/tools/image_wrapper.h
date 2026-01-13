#ifndef YUGGOTH_IMAGE_WRAPPER_H
#define YUGGOTH_IMAGE_WRAPPER_H

#include <filesystem>

namespace Yuggoth {

class ImageWrapper {
public:
  ImageWrapper(const std::filesystem::path &image_path);

  ~ImageWrapper();

  int32_t GetWidth() const;

  int32_t GetHeight() const;

  std::span<const std::byte> GetData() const;

private:
  int32_t width_ = 0;
  int32_t height_ = 0;
  int32_t channels_ = 0;
  uint8_t *image_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_WRAPPER_H