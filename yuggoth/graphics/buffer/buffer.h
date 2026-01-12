#ifndef YUGGOTH_BUFFER_H
#define YUGGOTH_BUFFER_H

#include "yuggoth/graphics_device/graphics_device_types.h"
#include "buffer_create_information.h"
#include <span>

namespace Yuggoth {

using BufferInformation = std::pair<VkBuffer, AllocationInformation>;

class Buffer {
public:
  Buffer() = default;

  Buffer(const BufferCreateInformation &buffer_ci);

  ~Buffer();

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;

  std::size_t GetSize() const;

  void SetRawData(std::span<const std::byte> data, std::size_t byte_offset = 0);

  template <typename T> std::span<T> GetMappedAs();
  template <typename T> void SetData(std::span<const T> data, std::size_t byte_offset = 0);

  void MemoryCopy(std::span<const std::byte> data, std::size_t byte_offset = 0);

  void Map();
  void Unmap();
  void Destroy();
  void Resize(std::size_t size);

  VkBuffer GetHandle() const;
  VkDeviceAddress GetDeviceAddress() const;

  Walle::BufferUsageMask GetUsage() const;
  Walle::MemoryPropertyMask GetMemoryPropertyMask() const;

  bool IsAccessWithCPU() const;
  bool IsGPU() const;

  static VkDeviceAddress GetBufferDeviceAddress(VkBuffer buffer);
  static BufferInformation CreateBuffer(const BufferCreateInformation &buffer_ci);

protected:
  void Swap(Buffer &other) noexcept;

private:
  VmaAllocation allocation_{VK_NULL_HANDLE};
  VkBuffer buffer_{VK_NULL_HANDLE};
  uint64_t buffer_size_{0};
  uint32_t memory_type_{0};
  uint8_t *mapped_memory_{nullptr};
  Walle::BufferUsageMask buffer_usage_;
  Walle::MemoryPropertyMask memory_property_;
};

} // namespace Yuggoth

#include "buffer.ipp"

#endif // YUGGOTH_BUFFER_H