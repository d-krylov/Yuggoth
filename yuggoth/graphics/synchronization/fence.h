#ifndef YUGGOTH_FENCE_H
#define YUGGOTH_FENCE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class Fence {
public:
  Fence(FenceCreateMask fence_create_mask = FenceCreateMask());

  ~Fence();

  Fence(const Fence &) = delete;
  Fence &operator=(const Fence &) = delete;

  Fence(Fence &&other) noexcept;
  Fence &operator=(Fence &&other) noexcept;

  void Wait();

  void Reset();

  VkFence GetHandle() const;

private:
  VkFence fence_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_FENCE_H