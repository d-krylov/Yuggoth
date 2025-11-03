#ifndef YUGGOTH_SYNCHRONIZATION_H
#define YUGGOTH_SYNCHRONIZATION_H

#include "graphics_structures.h"

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
  const VkSemaphore *GetPointer() const;

private:
  VkSemaphore semaphore_{VK_NULL_HANDLE};
};

class Fence {
public:
  Fence(FenceCreateMask mask);

  ~Fence();

  Fence(const Fence &) = delete;
  Fence &operator=(const Fence &) = delete;

  Fence(Fence &&other) noexcept;
  Fence &operator=(Fence &&other) noexcept;

  void Reset();
  void Wait();

  VkFence GetHandle() const;

private:
  VkFence fence_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_SYNCHRONIZATION_H