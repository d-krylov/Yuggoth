#include "swapchain.h"
#include "yuggoth/graphics/image/image.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include <algorithm>
#include <span>
#include <limits>
#include <GLFW/glfw3.h>

namespace Yuggoth {

std::vector<const char *> Swapchain::GetSwapchainExtensions() {
  uint32_t extensions_count = 0;
  auto status = glfwInit();
  auto extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
  return std::vector(extensions, extensions + extensions_count);
}

uint32_t ComputeImageCount(const VkSurfaceCapabilitiesKHR &capabilities) {
  constexpr auto infinity = std::numeric_limits<uint32_t>::max();
  return std::min(capabilities.minImageCount + 1, capabilities.maxImageCount > 0 ? capabilities.maxImageCount : infinity);
}

SurfaceFormatKHR SelectSurfaceFormat(const VkSurfaceKHR surface, std::span<const SurfaceFormatKHR> required_formats) {
  auto supported_formats = Enumerate<SurfaceFormatKHR>(vkGetPhysicalDeviceSurfaceFormatsKHR, GraphicsDevice::Get()->GetPhysicalDevice(), surface);
  auto it = std::ranges::find_first_of(supported_formats, required_formats);
  return (it != supported_formats.end()) ? *it : supported_formats[0];
}

PresentModeKHR SelectPresentMode(const VkSurfaceKHR surface) {
  std::array required_modes = {PresentModeKHR::E_MAILBOX_KHR, PresentModeKHR::E_IMMEDIATE_KHR, PresentModeKHR::E_FIFO_KHR};
  auto supported_modes = Enumerate<PresentModeKHR>(vkGetPhysicalDeviceSurfacePresentModesKHR, GraphicsDevice::Get()->GetPhysicalDevice(), surface);
  auto it = std::ranges::find_first_of(required_modes, supported_modes);
  return *it;
}

SurfaceCapabilitiesKHR GetSurfaceCapabilities(const VkSurfaceKHR surface) {
  SurfaceCapabilitiesKHR surface_capabilities{};
  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GraphicsDevice::Get()->GetPhysicalDevice(), surface, surface_capabilities));
  return surface_capabilities;
}

void Swapchain::CreateSurface(GLFWwindow *native_window) {
  VK_CHECK(glfwCreateWindowSurface(GraphicsDevice::Get()->GetInstance(), native_window, nullptr, &surface_));

  std::vector<SurfaceFormatKHR> required_formats{{Format::E_R8G8B8A8_SRGB, ColorSpaceKHR::E_SRGB_NONLINEAR_KHR},
                                                 {Format::E_B8G8R8A8_SRGB, ColorSpaceKHR::E_SRGB_NONLINEAR_KHR}};

  surface_format_ = SelectSurfaceFormat(surface_, required_formats);
}

Swapchain::Swapchain(GLFWwindow *native_window) {
  CreateSurface(native_window);
  CreateSwapchain();
  images_ = Enumerate<VkImage>(vkGetSwapchainImagesKHR, GraphicsDevice::Get()->GetDevice(), swapchain_current_);
  image_views_.resize(images_.size());
  auto subresource = GetImageSubresourceRange();
  for (auto i = 0; i < images_.size(); i++) {
    image_views_[i] = Image::CreateImageView(images_[i], surface_format_.format, ImageViewType::E_2D, subresource);
  }
}

Swapchain::~Swapchain() {
}

void Swapchain::CreateSwapchain() {
  auto surface_capabilities = GetSurfaceCapabilities(surface_);
  surface_extent_ = surface_capabilities.currentExtent;

  SwapchainCreateInfoKHR swapchain_ci{};
  swapchain_ci.surface = surface_;
  swapchain_ci.minImageCount = ComputeImageCount(surface_capabilities);
  swapchain_ci.imageFormat = surface_format_.format;
  swapchain_ci.imageColorSpace = surface_format_.colorSpace;
  swapchain_ci.imageExtent = surface_capabilities.currentExtent;
  swapchain_ci.imageArrayLayers = 1;
  swapchain_ci.imageUsage = ImageUsageMaskBits::E_COLOR_ATTACHMENT_BIT;
  swapchain_ci.imageSharingMode = SharingMode::E_EXCLUSIVE;
  swapchain_ci.preTransform = surface_capabilities.currentTransform;
  swapchain_ci.compositeAlpha = CompositeAlphaMaskBitsKHR::E_OPAQUE_BIT_KHR;
  swapchain_ci.presentMode = SelectPresentMode(surface_);
  swapchain_ci.clipped = true;
  swapchain_ci.oldSwapchain = swapchain_previous_;

  VK_CHECK(vkCreateSwapchainKHR(GraphicsDevice::Get()->GetDevice(), swapchain_ci, nullptr, &swapchain_current_));
}

void Swapchain::Present(const VkSemaphore *wait_semaphore) {
  PresentInfoKHR present_info;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = wait_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapchain_current_;
  present_info.pImageIndices = &image_index_;

  auto status = vkQueuePresentKHR(GraphicsDevice::Get()->GetGraphicsQueue(), present_info);
}

void Swapchain::CreateImageViews() {
}

VkResult Swapchain::AcquireNextImage(const VkSemaphore semaphore) {
  auto status = vkAcquireNextImageKHR(GraphicsDevice::Get()->GetDevice(), swapchain_current_, UINT64_MAX, semaphore, nullptr, &image_index_);
  return status;
}

void Swapchain::Cleanup() {
  for (const auto &image_view : image_views_) {
    vkDestroyImageView(GraphicsDevice::Get()->GetDevice(), image_view, nullptr);
  }
  vkDestroySwapchainKHR(GraphicsDevice::Get()->GetDevice(), swapchain_current_, nullptr);
}

void Swapchain::Recreate() {
  vkDeviceWaitIdle(GraphicsDevice::Get()->GetDevice());
  Cleanup();
  CreateSwapchain();
  images_ = Enumerate<VkImage>(vkGetSwapchainImagesKHR, GraphicsDevice::Get()->GetDevice(), swapchain_current_);
  auto subresource = GetImageSubresourceRange();
  image_views_.resize(images_.size());

  CommandBuffer command_buffer(GraphicsDevice::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);

  for (auto i = 0; i < images_.size(); i++) {
    image_views_[i] = Image::CreateImageView(images_[i], surface_format_.format, ImageViewType::E_2D, subresource);
  }

  command_buffer.End();
  command_buffer.Submit();
}

VkImage Swapchain::GetImage(uint32_t index) const {
  return images_[index];
}

VkImageView Swapchain::GetImageView(uint32_t index) const {
  return image_views_[index];
}

std::span<const VkImageView> Swapchain::GetImageViews() const {
  return image_views_;
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

const Extent2D &Swapchain::GetExtent() const {
  return surface_extent_;
}

uint32_t Swapchain::GetImageIndex() const {
  return image_index_;
}

Format Swapchain::GetFormat() const {
  return surface_format_.format;
}

} // namespace Yuggoth