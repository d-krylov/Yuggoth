#include "image2D.h"
#include "yuggoth/core/tools/include/image_wrapper.h"

namespace Yuggoth {

Image2D::Image2D(const std::filesystem::path &image_path, const std::optional<SamplerSpecification> &sampler_specification) {
  ImageWrapper image_wrapper(image_path);
  ImageUsageMask usage_mask = ImageUsageMaskBits::E_SAMPLED_BIT | ImageUsageMaskBits::E_TRANSFER_DST_BIT;
  Create(image_wrapper.GetWidth(), image_wrapper.GetHeight(), Format::E_R8G8B8A8_UNORM, usage_mask, sampler_specification);
  SetImageData(image_wrapper.GetData());
}

Image2D::Image2D(uint32_t width, uint32_t height, std::span<const std::byte> data, const std::optional<SamplerSpecification> &sampler_specification) {
  ImageUsageMask usage_mask = ImageUsageMaskBits::E_SAMPLED_BIT | ImageUsageMaskBits::E_TRANSFER_DST_BIT;
  Create(width, height, Format::E_R8G8B8A8_UNORM, usage_mask, sampler_specification);
  SetImageData(data);
}

Image2D::Image2D(uint32_t width, uint32_t height) {
  ImageUsageMask usage_mask = ImageUsageMaskBits::E_SAMPLED_BIT | ImageUsageMaskBits::E_COLOR_ATTACHMENT_BIT;
  Create(width, height, Format::E_R8G8B8A8_UNORM, usage_mask, SamplerSpecification());
}

void Image2D::Create(uint32_t width, uint32_t height, Format format, ImageUsageMask usage_mask,
                     const std::optional<SamplerSpecification> &sampler_specification) {
  ImageSpecification image_specification;
  image_specification.extent_ = Extent3D(width, height, 1);
  image_specification.format_ = format;
  image_specification.usage_ = usage_mask;
  Initialize(ImageType::E_2D, ImageViewType::E_2D, image_specification, sampler_specification);
}

Extent2D Image2D::GetExtent() const {
  auto specification = GetSpecification();
  auto extent = specification.extent_;
  return Extent2D(extent.width, extent.height);
}

} // namespace Yuggoth