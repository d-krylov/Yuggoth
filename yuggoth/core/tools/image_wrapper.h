#ifndef YUGGOTH_IMAGE_WRAPPER_H
#define YUGGOTH_IMAGE_WRAPPER_H

#include <filesystem>
#include "yuggoth_macros.h"

namespace Yuggoth {

class ImageWrapper {
public:
  DISABLE_COPY_SEMANTICS(ImageWrapper)

  ImageWrapper(const std::filesystem::path &image_path);

  ~ImageWrapper();

  ImageWrapper(ImageWrapper &&other) noexcept;
  ImageWrapper &operator=(ImageWrapper &&other) noexcept;

  ImageWrapper();

  int32_t GetWidth() const;
  int32_t GetHeight() const;

  std::span<const std::byte> GetData() const;

protected:
  void Swap(ImageWrapper &other);

private:
  int32_t width_ = 0;
  int32_t height_ = 0;
  int32_t channels_ = 0;
  uint8_t *image_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_WRAPPER_H