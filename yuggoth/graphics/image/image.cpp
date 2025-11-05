#include "image.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include <algorithm>

namespace Yuggoth {

VkImageView Image::CreateImageView(VkImage image, Format format) {

  ImageViewCreateInfo image_view_ci;
  {
    image_view_ci.image = image;
    image_view_ci.viewType = ImageViewType::E_2D;
    image_view_ci.format = format;
    image_view_ci.subresourceRange = GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT);
  }

  VkImageView image_view = VK_NULL_HANDLE;

  VK_CHECK(vkCreateImageView(GraphicsContext::Get()->GetDevice(), image_view_ci, 0, &image_view));

  return image_view;
}

void Image::CreateImage(const Extent3D &extent, ImageType type, Format format, uint32_t mip_levels, uint32_t layers, ImageUsageMask usage) {
  ImageCreateInfo image_ci;
  {
    image_ci.imageType = type;
    image_ci.format = format;
    image_ci.extent = extent;
    image_ci.mipLevels = mip_levels;
    image_ci.arrayLayers = layers;
    image_ci.samples = SampleCountMaskBits::E_1_BIT;
    image_ci.tiling = ImageTiling::E_OPTIMAL;
    image_ci.usage = usage;
  }

  vma_allocation_ = GraphicsAllocator::Get()->AllocateImage(image_ci, image_);
}

VkSampler CreateSampler(Filter filter, SamplerMipmapMode mipmap_mode, SamplerAddressMode address_mode) {
  VkSampler sampler = VK_NULL_HANDLE;
  SamplerCreateInfo sampler_ci;
  {
    sampler_ci.magFilter = filter;
    sampler_ci.minFilter = filter;
    sampler_ci.mipmapMode = mipmap_mode;
    sampler_ci.addressModeU = address_mode;
    sampler_ci.addressModeV = address_mode;
    sampler_ci.addressModeW = address_mode;
    sampler_ci.anisotropyEnable = false;
    sampler_ci.maxAnisotropy = 1.0f;
    sampler_ci.minLod = -1000;
    sampler_ci.maxLod = 1000;
  }
  VK_CHECK(vkCreateSampler(GraphicsContext::Get()->GetDevice(), sampler_ci, 0, &sampler));
  return sampler;
}

Image::Image(const ImageSpecification &image_specification) {
  CreateImage(image_specification.extent, ImageType::E_2D, image_specification.format, 1, 1, image_specification.usage);
  image_view_ = CreateImageView(image_, image_specification.format);
  sampler_ = CreateSampler(Filter::E_LINEAR, SamplerMipmapMode::E_LINEAR, SamplerAddressMode::E_CLAMP_TO_EDGE);
  extent_ = image_specification.extent;
}

Image::Image(Image &&other) noexcept {
  image_ = std::exchange(other.image_, VK_NULL_HANDLE);
  vma_allocation_ = std::exchange(other.vma_allocation_, VK_NULL_HANDLE);
  image_view_ = std::exchange(other.image_view_, VK_NULL_HANDLE);
  sampler_ = std::exchange(other.sampler_, VK_NULL_HANDLE);
}

Image &Image::operator=(Image &&other) noexcept {
  std::swap(image_, other.image_);
  std::swap(vma_allocation_, other.vma_allocation_);
  std::swap(image_view_, other.image_view_);
  std::swap(sampler_, other.sampler_);
  extent_ = other.extent_;
  return *this;
}

const Extent3D &Image::GetExtent() const {
  return extent_;
}

VkImage Image::GetImage() const {
  return image_;
}

VkImageView Image::GetImageView() const {
  return image_view_;
}

VkSampler Image::GetSampler() const {
  return sampler_;
}

Image::~Image() {
  vkDestroyImage(GraphicsContext::Get()->GetDevice(), image_, nullptr);
  vkDestroyImageView(GraphicsContext::Get()->GetDevice(), image_view_, nullptr);
}

void Image::SetImageData(std::span<const std::byte> data) {
  Buffer buffer(data.size(), BufferUsageMaskBits::E_TRANSFER_SRC_BIT, true);
  std::ranges::copy(data, buffer.GetMapped().begin());
  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin();

  auto subresource = GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT);

  auto barrier1 =
    GetImageMemoryBarrier(GetImage(), ImageLayout::E_UNDEFINED, ImageLayout::E_TRANSFER_DST_OPTIMAL, PipelineStageMaskBits2::E_TOP_OF_PIPE_BIT,
                          PipelineStageMaskBits2::E_ALL_TRANSFER_BIT, {}, AccessMaskBits2::E_TRANSFER_READ_BIT, subresource);

  command_buffer.CommandPipelineBarrier({}, {}, std::array{barrier1});

  command_buffer.CommandCopyBufferToImage(buffer.GetHandle(), GetImage(), GetExtent());

  auto barrier2 = GetImageMemoryBarrier(GetImage(), ImageLayout::E_TRANSFER_DST_OPTIMAL, ImageLayout::E_READ_ONLY_OPTIMAL,
                                        PipelineStageMaskBits2::E_ALL_TRANSFER_BIT, PipelineStageMaskBits2::E_FRAGMENT_SHADER_BIT,
                                        AccessMaskBits2::E_TRANSFER_READ_BIT, AccessMaskBits2::E_SHADER_READ_BIT, subresource);

  command_buffer.CommandPipelineBarrier({}, {}, std::array{barrier2});

  command_buffer.End();

  command_buffer.Submit();
}

} // namespace Yuggoth