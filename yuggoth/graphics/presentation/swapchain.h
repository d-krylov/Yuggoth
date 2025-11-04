#ifndef YUGGOTH_SWAPCHAIN_H
#define YUGGOTH_SWAPCHAIN_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <span>

struct GLFWwindow;

namespace Yuggoth {

class Swapchain {
public:
  Swapchain(GLFWwindow *native_window);

  ~Swapchain();

  void Present(const VkSemaphore *wait_semaphore);

  VkImage GetImage(uint32_t index) const;
  VkImageView GetImageView(uint32_t index) const;

  VkImage GetCurrentImage() const;
  VkImageView GetCurrentImageView() const;

  uint32_t GetImageIndex() const;
  std::size_t GetNumberOfImages() const;
  const Extent2D &GetExtent() const;
  Format GetFormat() const;

  std::span<const VkImageView> GetImageViews() const;

  VkResult AcquireNextImage(const VkSemaphore semaphore);

  static std::vector<const char *> GetSwapchainExtensions();

  void Cleanup();
  void Recreate();

protected:
  void CreateSurface(GLFWwindow *native_window);
  void CreateSwapchain();

private:
  VkSurfaceKHR surface_{VK_NULL_HANDLE};
  VkSwapchainKHR swapchain_current_{VK_NULL_HANDLE};
  VkSwapchainKHR swapchain_previous_{VK_NULL_HANDLE};
  SurfaceFormatKHR surface_format_;
  Extent2D surface_extent_;
  std::vector<VkImage> images_;
  std::vector<VkImageView> image_views_;
  uint32_t image_index_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_SWAPCHAIN_H
