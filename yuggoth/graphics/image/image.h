#ifndef YUGGOTH_IMAGE_H
#define YUGGOTH_IMAGE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include <span>

namespace Yuggoth {

struct ImageSpecification {
  Extent3D extent;
  Format format;
  ImageType image_type;
  ImageUsageMask usage;
  uint32_t levels;
  uint32_t layers;
};

class Image {
public:
  Image(const ImageSpecification &image_specification);

  ~Image();

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  Image(Image &&other) noexcept;
  Image &operator=(Image &&other) noexcept;

  static VkImageView CreateImageView(VkImage image, Format format);

  const Extent3D &GetExtent() const;

  VkImage GetImage() const;
  VkImageView GetImageView() const;
  VkSampler GetSampler() const;

  void SetImageData(std::span<const std::byte> data);

protected:
  void CreateImage(const Extent3D &extent, ImageType type, Format format, uint32_t mip_levels, uint32_t array_layers, ImageUsageMask usage);

private:
  VkImage image_{VK_NULL_HANDLE};
  VmaAllocation vma_allocation_{VK_NULL_HANDLE};
  VkImageView image_view_{VK_NULL_HANDLE};
  VkSampler sampler_{VK_NULL_HANDLE};
  Extent3D extent_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_H
