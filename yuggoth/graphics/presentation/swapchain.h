#ifndef YUGGOTH_SWAPCHAIN_H
#define YUGGOTH_SWAPCHAIN_H

#include "surface.h"
#include <vector>

namespace Yuggoth {

class Swapchain {
public:
  Swapchain(const Surface &surface);

  ~Swapchain();

  void Present(const VkSemaphore *wait_semaphore);

  VkImage GetImage(uint32_t index) const;
  VkImageView GetImageView(uint32_t index) const;

  VkImage GetCurrentImage() const;
  VkImageView GetCurrentImageView() const;

  uint32_t GetImageIndex() const;
  std::size_t GetNumberOfImages() const;

  VkResult AcquireNextImage(const VkSemaphore semaphore);

  void Cleanup();
  void Recreate(const Surface &surface);

protected:
  void CreateSwapchain(const Surface &surface);

private:
  VkSwapchainKHR swapchain_current_{VK_NULL_HANDLE};
  VkSwapchainKHR swapchain_previous_{VK_NULL_HANDLE};
  std::vector<VkImage> images_;
  std::vector<VkImageView> image_views_;
  uint32_t image_index_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_SWAPCHAIN_H
