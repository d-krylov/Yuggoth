#ifndef YUGGOTH_BUFFER_H
#define YUGGOTH_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include <span>

namespace Yuggoth {

struct BufferInformation {
  VkBuffer buffer_{VK_NULL_HANDLE};
  VmaAllocation allocation_{VK_NULL_HANDLE};
  std::byte *mapped_memory_{nullptr};
};

class Buffer {
public:
  static constexpr AllocationCreateMask CPU = AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
  static constexpr AllocationCreateMask MAPPED = CPU | AllocationCreateMaskBits::E_MAPPED_BIT;

  Buffer() = default;

  Buffer(std::size_t buffer_size, BufferUsageMask buffer_usage, AllocationCreateMask allocation_mask);

  ~Buffer();

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;

  std::size_t GetSize() const;

  template <typename T> std::span<T> GetMappedAs();
  template <typename T> void SetData(std::span<const T> data, std::size_t byte_offset = 0);

  void Map();
  void Unmap();
  void Destroy();

  VkBuffer GetHandle() const;
  VkDeviceAddress GetBufferDeviceAddress() const;

  static BufferInformation CreateBuffer(std::size_t size, BufferUsageMask usage, AllocationCreateMask allocation_mask);

private:
  VkBuffer buffer_{VK_NULL_HANDLE};
  VmaAllocation allocation_{VK_NULL_HANDLE};
  std::size_t buffer_size_{0};
  std::byte *mapped_memory_{nullptr};
};

} // namespace Yuggoth

#include "buffer.ipp"

#endif // YUGGOTH_BUFFER_H