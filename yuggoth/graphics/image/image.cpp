#include "image.h"
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

VkImageView Image::CreateImageView(VkImage image, VkFormat format) {

  VkImageViewCreateInfo image_view_ci{};
  {
    image_view_ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_ci.image = image;
    image_view_ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_ci.format = format;
    image_view_ci.subresourceRange = GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT);
  }

  VkImageView image_view = VK_NULL_HANDLE;

  VK_CHECK(vkCreateImageView(GraphicsContext::Get()->GetDevice(), &image_view_ci, 0, &image_view));

  return image_view;
}

void CreateImage(const VkExtent3D &extent, Format format, uint32_t mip_levels, VkImageUsageFlags usage) {
  VkImageCreateInfo image_ci{};
  {
    image_ci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_ci.imageType = VK_IMAGE_TYPE_2D;
    image_ci.format = (VkFormat)format;
    image_ci.extent = extent;
    image_ci.mipLevels = mip_levels;
    image_ci.arrayLayers = 1;
    image_ci.samples = VK_SAMPLE_COUNT_1_BIT;
    image_ci.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_ci.usage = usage;
  }

  // vma_allocation_ = GraphicsAllocator::Get()->AllocateBuffer(image_ci, buffer_);
}

VkSampler CreateSampler(VkFilter filter, VkSamplerMipmapMode mipmap_mode, VkSamplerAddressMode address_mode) {
  VkSampler sampler = VK_NULL_HANDLE;
  VkSamplerCreateInfo sampler_ci{};
  {
    sampler_ci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_ci.magFilter = filter;
    sampler_ci.minFilter = filter;
    sampler_ci.mipmapMode = mipmap_mode;
    sampler_ci.addressModeU = address_mode;
    sampler_ci.addressModeV = address_mode;
    sampler_ci.addressModeW = address_mode;
    sampler_ci.anisotropyEnable = mipmap_mode == VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_ci.maxAnisotropy = (mipmap_mode == VK_SAMPLER_MIPMAP_MODE_LINEAR) ? 4.0f : 1.0f;
    sampler_ci.minLod = 0.0f;
    sampler_ci.maxLod = 16.0f;
  }
  VK_CHECK(vkCreateSampler(GraphicsContext::Get()->GetDevice(), &sampler_ci, 0, &sampler));
  return sampler;
}

} // namespace Yuggoth