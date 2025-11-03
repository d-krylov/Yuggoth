#ifndef YUGGOTH_IMAGE_H
#define YUGGOTH_IMAGE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"

namespace Yuggoth {

class Image {
public:
  Image();

  ~Image();

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  static VkImageView CreateImageView(VkImage image, VkFormat format);

protected:
private:
  VkImage image_{VK_NULL_HANDLE};
  VmaAllocation vma_allocation_{VK_NULL_HANDLE};
  VkImageView image_view_{VK_NULL_HANDLE};
  VkSampler sampler_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_IMAGE_H
