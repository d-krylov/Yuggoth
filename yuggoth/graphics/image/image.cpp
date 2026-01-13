#include "image.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/graphics_allocator.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include "yuggoth/graphics/core/graphics_types.h"
#include <algorithm>

namespace Yuggoth {

VkImageView Image::CreateImageView(VkImage image, const ImageViewCreateInformation &image_view_create_information) {
  Walle::ImageViewCreateInfo image_view_ci;
  image_view_ci.image = image;
  image_view_ci.viewType = image_view_create_information.image_view_type_;
  image_view_ci.format = image_view_create_information.format_;
  image_view_ci.subresourceRange.aspectMask = image_view_create_information.aspect_;
  image_view_ci.subresourceRange.baseMipLevel = image_view_create_information.base_level_;
  image_view_ci.subresourceRange.baseArrayLayer = image_view_create_information.base_layer_;
  image_view_ci.subresourceRange.levelCount = image_view_create_information.level_count_;
  image_view_ci.subresourceRange.layerCount = image_view_create_information.layer_count_;
  VkImageView image_view = VK_NULL_HANDLE;
  VK_CHECK(vkCreateImageView(GraphicsContext::Get()->GetDevice(), image_view_ci, nullptr, &image_view));
  return image_view;
}

ImageAllocationInformation Image::CreateImage(const ImageCreateInformation &image_create_information) {
  Walle::ImageCreateInfo image_ci;
  image_ci.imageType = image_create_information.image_type_;
  image_ci.extent = image_create_information.extent_;
  image_ci.mipLevels = image_create_information.levels_;
  image_ci.arrayLayers = image_create_information.layers_;
  image_ci.format = image_create_information.format_;
  image_ci.tiling = ImageTiling::E_OPTIMAL;
  image_ci.initialLayout = ImageLayout::E_UNDEFINED;
  image_ci.usage = image_create_information.usage_;
  image_ci.samples = image_create_information.samples_;
  image_ci.sharingMode = SharingMode::E_EXCLUSIVE;

  AllocationCreateInformation allocation_ci;
  allocation_ci.required_memory_property_ = Walle::MemoryPropertyMaskBits::E_DEVICE_LOCAL_BIT;
  allocation_ci.allocation_create_mask_ = Walle::AllocationCreateMaskBits::E_DEDICATED_MEMORY_BIT;

  auto image_allocation_information = GraphicsAllocator::Get()->AllocateImage(image_ci, allocation_ci);
  return image_allocation_information;
}

void Image::CreateImage() {
  auto image_allocation_information = CreateImage(image_create_information_);
  image_ = image_allocation_information.image_handle_;
  allocation_ = image_allocation_information.allocation_;
  auto image_view_ci = ImageViewCreateInformation::CreateDefault(image_create_information_.image_view_type_, image_create_information_.format_);
  image_view_ = CreateImageView(image_, image_view_ci);
  current_layout_ = ImageLayout::E_UNDEFINED;
}

Image::Image(const ImageCreateInformation &image_ci, const std::optional<SamplerCreateInformation> &sampler_specification)
  : image_create_information_(image_ci) {
  CreateImage();
  image_sampler_ = sampler_specification.transform(Sampler::CreateSampler).value_or(VK_NULL_HANDLE);
}

Image::Image(Image &&other) noexcept {
  Swap(other);
}

Image &Image::operator=(Image &&other) noexcept {
  Swap(other);
  return *this;
}

void Image::Swap(Image &other) noexcept {
  std::swap(image_, other.image_);
  std::swap(allocation_, other.allocation_);
  std::swap(image_view_, other.image_view_);
  std::swap(image_sampler_, other.image_sampler_);
  std::swap(current_layout_, other.current_layout_);
  std::swap(image_create_information_, other.image_create_information_);
}

Image::~Image() {
  DestroyImage();
  DestroySampler();
}

void Image::DestroyImage() {
  vkDestroyImageView(GraphicsContext::Get()->GetDevice(), image_view_, nullptr);
  GraphicsAllocator::Get()->DestroyImage(image_, allocation_);
}

void Image::DestroySampler() {
  vkDestroySampler(GraphicsContext::Get()->GetDevice(), image_sampler_, nullptr);
}

// API
void Image::Resize(uint32_t width, uint32_t heigth) {
  DestroyImage();
  image_create_information_.extent_.width = width;
  image_create_information_.extent_.height = heigth;
  CreateImage();
}

VkImage Image::GetImage() const {
  return image_;
}

VkImageView Image::GetImageView() const {
  return image_view_;
}

VkSampler Image::GetSampler() const {
  return image_sampler_;
}

Walle::ImageLayout Image::GetImageLayout() const {
  return current_layout_;
}

const ImageCreateInformation &Image::GetImageCreateInformation() const {
  return image_create_information_;
}

void Image::SetImageLayout(ImageLayout new_layout, CommandBuffer *command_buffer) {
  if (new_layout == current_layout_) return;
  auto access0 = GetAccessMaskFromLayout(current_layout_, false);
  auto access1 = GetAccessMaskFromLayout(new_layout, true);
  auto stage0 = GetPipelineStageMaskFromLayout(current_layout_, false);
  auto stage1 = GetPipelineStageMaskFromLayout(new_layout, true);
  auto subresource = GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT);
  command_buffer->TransitionImageLayout(GetImage(), current_layout_, new_layout, stage0, stage1, access0, access1, subresource);
  current_layout_ = new_layout;
}

void Image::SetImageData(std::span<const std::byte> data) {
  Buffer buffer(BufferCreateInformation::CreateStagingBuffer(data.size_bytes()));
  buffer.SetData<std::byte>(data);
  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  SetImageLayout(ImageLayout::E_TRANSFER_DST_OPTIMAL, &command_buffer);
  command_buffer.CommandCopyBufferToImage(buffer.GetHandle(), GetImage(), image_create_information_.extent_);
  SetImageLayout(ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer);
  command_buffer.End();
  command_buffer.Submit();
}

Walle::DescriptorImageInfo Image::GetDescriptor() const {
  Walle::DescriptorImageInfo descriptor_image_info;
  descriptor_image_info.imageLayout = current_layout_;
  descriptor_image_info.imageView = GetImageView();
  descriptor_image_info.sampler = GetSampler();
  return descriptor_image_info;
}

AssetKind Image::GetAssetKind() const {
  return AssetKind::IMAGE;
}

} // namespace Yuggoth