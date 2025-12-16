#ifndef YUGGOTH_SEMAPHORE_H
#define YUGGOTH_SEMAPHORE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class Semaphore {
public:
  Semaphore();

  ~Semaphore();

  Semaphore(const Semaphore &) = delete;
  Semaphore &operator=(const Semaphore &) = delete;

  Semaphore(Semaphore &&other) noexcept;
  Semaphore &operator=(Semaphore &&other) noexcept;

  VkSemaphore GetHandle() const;

  const VkSemaphore *get() const;

private:
  VkSemaphore semaphore_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_SEMAPHORE_H