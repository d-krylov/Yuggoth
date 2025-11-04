#ifndef YUGGOTH_BUFFER_H
#define YUGGOTH_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include <span>

namespace Yuggoth {

class Buffer {
public:
  Buffer() = default;

  Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage, bool mapped = false);

  ~Buffer();

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;

  std::size_t GetSize() const;

  template <typename T>
  std::span<T> GetMappedAs();

  std::span<std::byte> GetMapped();

  void Map();
  void Unmap();

  VkBuffer GetHandle() const;

private:
  VkBuffer buffer_{VK_NULL_HANDLE};
  VmaAllocation vma_allocation_{VK_NULL_HANDLE};
  std::size_t buffer_size_{0};
  std::byte *mapped_memory_{nullptr};
};

} // namespace Yuggoth

#include "buffer.ipp"

#endif // YUGGOTH_BUFFER_H