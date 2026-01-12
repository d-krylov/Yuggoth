#include "fence.h"
#include "yuggoth/graphics_device/graphics_device.h"
#include <utility>

namespace Yuggoth {

Fence::Fence(Walle::FenceCreateMask fence_create_mask) {
  Walle::FenceCreateInfo fence_ci;
  fence_ci.flags = fence_create_mask;
  VK_CHECK(vkCreateFence(GraphicsDevice::Get()->GetDevice(), fence_ci, nullptr, &fence_));
}

Fence::~Fence() {
  vkDestroyFence(GraphicsDevice::Get()->GetDevice(), fence_, nullptr);
}

Fence::Fence(Fence &&other) noexcept {
  fence_ = std::exchange(other.fence_, VK_NULL_HANDLE);
}

Fence &Fence::operator=(Fence &&other) noexcept {
  std::swap(fence_, other.fence_);
  return *this;
}

void Fence::Wait() {
  VK_CHECK(vkWaitForFences(GraphicsDevice::Get()->GetDevice(), 1, &fence_, VK_TRUE, UINT64_MAX));
}

void Fence::Reset() {
  VK_CHECK(vkResetFences(GraphicsDevice::Get()->GetDevice(), 1, &fence_));
}

VkFence Fence::GetHandle() const {
  return fence_;
}

} // namespace Yuggoth