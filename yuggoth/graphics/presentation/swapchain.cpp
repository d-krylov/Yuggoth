#include "swapchain.h"
#include "yuggoth/graphics/image/image.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/core/tools/type_tools.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include <algorithm>
#include <span>
#include <limits>
#include <GLFW/glfw3.h>

namespace Yuggoth {

uint32_t ComputeImageCount(const VkSurfaceCapabilitiesKHR &capabilities) {
  constexpr auto infinity = std::numeric_limits<uint32_t>::max();
  return std::min(capabilities.minImageCount + 1, capabilities.maxImageCount > 0 ? capabilities.maxImageCount : infinity);
}

Swapchain::Swapchain(const Surface &surface) {
  CreateSwapchain(surface);
  images_ = Enumerate<VkImage>(vkGetSwapchainImagesKHR, GraphicsContext::Get()->GetDevice(), swapchain_current_);
  image_views_.resize(images_.size());
  auto surface_format = surface.GetSurfaceFormat();
  auto image_view_ci = ImageViewCreateInformation::CreateDefault(Walle::ImageViewType::E_2D, surface_format.format);
  for (auto i = 0; i < images_.size(); i++) {
    image_views_[i] = Image::CreateImageView(images_[i], image_view_ci);
  }
}

Swapchain::~Swapchain() {
}

void Swapchain::CreateSwapchain(const Surface &surface) {
  auto surface_capabilities = surface.GetSurfaceCapabilities();
  auto surface_format = surface.GetSurfaceFormat();

  Walle::SwapchainCreateInfoKHR swapchain_ci;
  swapchain_ci.surface = surface.GetHandle();
  swapchain_ci.minImageCount = ComputeImageCount(surface_capabilities);
  swapchain_ci.imageFormat = surface_format.format;
  swapchain_ci.imageColorSpace = surface_format.colorSpace;
  swapchain_ci.imageExtent = surface_capabilities.currentExtent;
  swapchain_ci.imageArrayLayers = 1;
  swapchain_ci.imageUsage = Walle::ImageUsageMaskBits::E_COLOR_ATTACHMENT_BIT;
  swapchain_ci.imageSharingMode = Walle::SharingMode::E_EXCLUSIVE;
  swapchain_ci.preTransform = surface_capabilities.currentTransform;
  swapchain_ci.compositeAlpha = Walle::CompositeAlphaMaskBitsKHR::E_OPAQUE_BIT_KHR;
  swapchain_ci.presentMode = surface.GetPresentMode();
  swapchain_ci.clipped = true;
  swapchain_ci.oldSwapchain = swapchain_previous_;

  VK_CHECK(vkCreateSwapchainKHR(GraphicsContext::Get()->GetDevice(), swapchain_ci, nullptr, &swapchain_current_));
}

void Swapchain::Present(const VkSemaphore *wait_semaphore) {
  Walle::PresentInfoKHR present_info;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = wait_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapchain_current_;
  present_info.pImageIndices = &image_index_;

  auto status = vkQueuePresentKHR(GraphicsContext::Get()->GetGraphicsQueue(), present_info);
}

VkResult Swapchain::AcquireNextImage(const VkSemaphore semaphore) {
  auto status = vkAcquireNextImageKHR(GraphicsContext::Get()->GetDevice(), swapchain_current_, UINT64_MAX, semaphore, nullptr, &image_index_);
  return status;
}

void Swapchain::Cleanup() {
  for (const auto &image_view : image_views_) {
    vkDestroyImageView(GraphicsContext::Get()->GetDevice(), image_view, nullptr);
  }
  vkDestroySwapchainKHR(GraphicsContext::Get()->GetDevice(), swapchain_current_, nullptr);
}

void Swapchain::Recreate(const Surface &surface) {
  vkDeviceWaitIdle(GraphicsContext::Get()->GetDevice());
  Cleanup();
  CreateSwapchain(surface);
  images_ = Enumerate<VkImage>(vkGetSwapchainImagesKHR, GraphicsContext::Get()->GetDevice(), swapchain_current_);
  auto surface_format = surface.GetSurfaceFormat();
  auto image_view_ci = ImageViewCreateInformation::CreateDefault(Walle::ImageViewType::E_2D, surface_format.format);
  image_views_.resize(images_.size());
  for (auto i = 0; i < images_.size(); i++) {
    image_views_[i] = Image::CreateImageView(images_[i], image_view_ci);
  }
}

VkImage Swapchain::GetImage(uint32_t index) const {
  return images_[index];
}

VkImageView Swapchain::GetImageView(uint32_t index) const {
  return image_views_[index];
}

VkImage Swapchain::GetCurrentImage() const {
  return GetImage(image_index_);
}

VkImageView Swapchain::GetCurrentImageView() const {
  return GetImageView(image_index_);
}

std::size_t Swapchain::GetNumberOfImages() const {
  return image_views_.size();
}

uint32_t Swapchain::GetImageIndex() const {
  return image_index_;
}

} // namespace Yuggoth