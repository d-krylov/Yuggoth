
#include "semaphore.h"
#include "yuggoth/graphics_device/graphics_device.h"

namespace Yuggoth {

Semaphore::Semaphore() {
  Walle::SemaphoreCreateInfo semaphore_ci;
  VK_CHECK(vkCreateSemaphore(GraphicsDevice::Get()->GetDevice(), semaphore_ci, nullptr, &semaphore_));
}

Semaphore::~Semaphore() {
  vkDestroySemaphore(GraphicsDevice::Get()->GetDevice(), semaphore_, nullptr);
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

const VkSemaphore *Semaphore::get() const {
  return &semaphore_;
}

} // namespace Yuggoth
