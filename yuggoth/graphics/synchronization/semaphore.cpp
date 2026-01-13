
#include "semaphore.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Semaphore::Semaphore() {
  Walle::SemaphoreCreateInfo semaphore_ci;
  VK_CHECK(vkCreateSemaphore(GraphicsContext::Get()->GetDevice(), semaphore_ci, nullptr, &semaphore_));
}

Semaphore::~Semaphore() {
  vkDestroySemaphore(GraphicsContext::Get()->GetDevice(), semaphore_, nullptr);
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
