#ifndef YUGGOTH_IMAGE_H
#define YUGGOTH_IMAGE_H

#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include "yuggoth/asset/core/asset.h"
#include "sampler.h"
#include <optional>
#include <span>

namespace Yuggoth {

class CommandBuffer;

class Image : public Asset {
public:
  Image() = default;

  Image(ImageType type, ImageViewType view_type, const ImageSpecification &image_specification,
        const std::optional<SamplerSpecification> &sampler_specification);

  ~Image();

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  Image(Image &&other) noexcept;
  Image &operator=(Image &&other) noexcept;

  void Destroy();

  static VkImageView CreateImageView(VkImage image, Format format, ImageViewType image_view_type, const ImageSubresourceRange &subresource);
  static VkImage CreateImage(ImageType image_type, const ImageSpecification &image_specification, VmaAllocation &out_allocation);

  // API
  VkImage GetImage() const;
  VkSampler GetSampler() const;
  VkImageView GetImageView() const;

  const ImageSpecification &GetSpecification() const;

  void SetImageData(std::span<const std::byte> data);
  void SetImageLayout(ImageLayout new_layout, CommandBuffer *command_buffer);

  DescriptorImageInfo GetDescriptor() const;

protected:
  void Swap(Image &other) noexcept;

  void Initialize(ImageType type, ImageViewType view_type, const ImageSpecification &image_specification,
                  const std::optional<SamplerSpecification> &sampler_specification = std::nullopt);

private:
  VkImage image_{VK_NULL_HANDLE};
  VmaAllocation allocation_{VK_NULL_HANDLE};
  VkImageView image_view_{VK_NULL_HANDLE};
  VkSampler image_sampler_{VK_NULL_HANDLE};
  ImageLayout current_layout_ = ImageLayout::E_UNDEFINED;
  ImageSpecification image_specification_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_H
