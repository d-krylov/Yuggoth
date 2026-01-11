#ifndef YUGGOTH_IMAGE_H
#define YUGGOTH_IMAGE_H

#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include "yuggoth/asset/core/asset.h"
#include "image_create_information.h"
#include "sampler.h"
#include <optional>
#include <span>

namespace Yuggoth {

class CommandBuffer;

class Image : public Asset {
public:
  Image() = default;

  Image(const ImageCreateInformation &image_ci, const std::optional<SamplerSpecification> &sampler_specification);

  ~Image();

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  Image(Image &&other) noexcept;
  Image &operator=(Image &&other) noexcept;

  static VkImageView CreateImageView(VkImage image, Format format, ImageViewType image_view_type, const ImageSubresourceRange &subresource);
  static VkImage CreateImage(const ImageCreateInformation &image_ci, VmaAllocation &out_allocation);

  // API
  VkImage GetImage() const;
  VkSampler GetSampler() const;
  VkImageView GetImageView() const;

  AssetKind GetAssetKind() const override;

  Walle::ImageLayout GetImageLayout() const;

  const ImageCreateInformation &GetImageCreateInformation() const;

  void SetImageData(std::span<const std::byte> data);
  void SetImageLayout(ImageLayout new_layout, CommandBuffer *command_buffer);

  void Resize(uint32_t width, uint32_t heigth);

  DescriptorImageInfo GetDescriptor() const;

protected:
  void Swap(Image &other) noexcept;
  void DestroyImage();
  void DestroySampler();

  void CreateImage();

private:
  VkImage image_{VK_NULL_HANDLE};
  VmaAllocation allocation_{VK_NULL_HANDLE};
  VkImageView image_view_{VK_NULL_HANDLE};
  VkSampler image_sampler_{VK_NULL_HANDLE};
  Walle::ImageLayout current_layout_ = Walle::ImageLayout::E_UNDEFINED;
  ImageCreateInformation image_create_information_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_H
