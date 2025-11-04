#include "synchronization.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

Semaphore::Semaphore() {
  SemaphoreCreateInfo semaphore_ci;
  VK_CHECK(vkCreateSemaphore(GraphicsContext::Get()->GetDevice(), semaphore_ci, 0, &semaphore_));
}

Semaphore::~Semaphore() {
}

Semaphore::Semaphore(Semaphore &&other) noexcept {
  semaphore_ = std::exchange(other.semaphore_, VK_NULL_HANDLE);
}

Semaphore &Semaphore::operator=(Semaphore &&other) noexcept {
  std::swap(semaphore_, other.semaphore_);
  return *this;
}

VkSemaphore Semaphore::GetHandle() const {
  return semaphore_;
}

const VkSemaphore *Semaphore::GetPointer() const {
  return &semaphore_;
}

Fence::Fence(FenceCreateMask mask) {
  FenceCreateInfo fence_ci;
  fence_ci.flags = mask;
  VK_CHECK(vkCreateFence(GraphicsContext::Get()->GetDevice(), fence_ci, 0, &fence_));
}

Fence::~Fence() {
}

Fence::Fence(Fence &&other) noexcept {
  fence_ = std::exchange(other.fence_, VK_NULL_HANDLE);
}

Fence &Fence::operator=(Fence &&other) noexcept {
  std::swap(fence_, other.fence_);
  return *this;
}

void Fence::Reset() {
  vkResetFences(GraphicsContext::Get()->GetDevice(), 1, &fence_);
}

void Fence::Wait() {
  vkWaitForFences(GraphicsContext::Get()->GetDevice(), 1, &fence_, VK_TRUE, UINT64_MAX);
}

VkFence Fence::GetHandle() const {
  return fence_;
}

} // namespace Yuggoth