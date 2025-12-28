#ifndef YUGGOTH_BUFFER_H
#define YUGGOTH_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include <span>

namespace Yuggoth {

using BufferInformation = std::pair<VkBuffer, AllocationInformation>;

class Buffer {
public:
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

  void MemoryCopy(std::span<const std::byte> data, std::size_t byte_offset = 0);

  void Map();
  void Unmap();
  void Destroy();

  VkBuffer GetHandle() const;
  VkDeviceAddress GetBufferDeviceAddress() const;

  BufferUsageMask GetUsage() const;
  MemoryPropertyMask GetMemoryPropertyMask() const;

  bool IsAccessWithCPU() const;
  bool IsGPU() const;

  static BufferInformation CreateBuffer(std::size_t size, BufferUsageMask usage, AllocationCreateMask allocation_mask);

protected:
  void Swap(Buffer &other) noexcept;

private:
  VmaAllocation allocation_{VK_NULL_HANDLE};
  VkBuffer buffer_{VK_NULL_HANDLE};
  uint64_t buffer_size_{0};
  uint32_t memory_type_{0};
  uint8_t *mapped_memory_{nullptr};
  BufferUsageMask buffer_usage_;
  MemoryPropertyMask memory_property_;
};

} // namespace Yuggoth

#include "buffer.ipp"

#endif // YUGGOTH_BUFFER_H