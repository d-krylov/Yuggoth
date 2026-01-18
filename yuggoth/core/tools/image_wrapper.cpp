#include "yuggoth/core/tools/image_wrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Yuggoth {

ImageWrapper::ImageWrapper(const std::filesystem::path &image_path) {
  image_ = stbi_load(image_path.c_str(), &width_, &height_, &channels_, STBI_rgb_alpha);
}

ImageWrapper::~ImageWrapper() {
  stbi_image_free(image_);
}

std::span<const std::byte> ImageWrapper::GetData() const {
  auto channels = 4;
  auto data = std::span(image_, width_ * height_ * channels);
  return std::as_bytes(data);
}

int32_t ImageWrapper::GetWidth() const {
  return width_;
}

int32_t ImageWrapper::GetHeight() const {
  return height_;
}

ImageWrapper::ImageWrapper(ImageWrapper &&other) noexcept {
  Swap(other);
}

ImageWrapper &ImageWrapper::operator=(ImageWrapper &&other) noexcept {
  Swap(other);
  return *this;
}

void ImageWrapper::Swap(ImageWrapper &other) {
  std::swap(width_, other.width_);
  std::swap(height_, other.height_);
  std::swap(channels_, other.channels_);
  std::swap(image_, other.image_);
}

} // namespace Yuggoth