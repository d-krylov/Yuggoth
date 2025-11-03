#ifndef YUGGOTH_BUFFER_H
#define YUGGOTH_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"

namespace Yuggoth {

class Buffer {
public:
  Buffer() = default;

  Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage);

  ~Buffer();

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;

  std::size_t GetSize() const;

private:
  VkBuffer buffer_{VK_NULL_HANDLE};
  VmaAllocation vma_allocation_{VK_NULL_HANDLE};
  std::size_t buffer_size_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_H