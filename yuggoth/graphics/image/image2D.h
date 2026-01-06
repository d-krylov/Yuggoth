#ifndef YUGGOTH_IMAGE_2D_H
#define YUGGOTH_IMAGE_2D_H

#include "image.h"
#include <filesystem>

namespace Yuggoth {

class Image2D : public Image {
public:
  Image2D() = default;

  Image2D(const std::filesystem::path &image_path, const std::optional<SamplerSpecification> &sampler_specification = std::nullopt);

  Image2D(uint32_t width, uint32_t height, std::span<const std::byte> data,
          const std::optional<SamplerSpecification> &sampler_specification = std::nullopt);

  Image2D(uint32_t width, uint32_t height);

  Image2D(uint32_t width, uint32_t height, Format format, ImageUsageMask optional_usage);

  Image2D(Image2D &&other) noexcept = default;

  Image2D &operator=(Image2D &&other) noexcept = default;

  Extent2D GetExtent() const;

protected:
  void Create(uint32_t width, uint32_t height, Format format, ImageUsageMask usage_mask,
              const std::optional<SamplerSpecification> &sampler_specification);
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_2D_H