#ifndef YUGGOTH_DESCRIPTOR_POOL_H
#define YUGGOTH_DESCRIPTOR_POOL_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <span>

namespace Yuggoth {

class DescriptorPool {
public:
  DescriptorPool() = default;

  DescriptorPool(std::span<const Walle::DescriptorPoolSize> descriptor_pool_sizes, Walle::DescriptorPoolCreateMask mask, uint32_t max_sets);

  ~DescriptorPool();

  DescriptorPool(const DescriptorPool &) = delete;
  DescriptorPool &operator=(const DescriptorPool &) = delete;

  DescriptorPool(DescriptorPool &&other) noexcept;
  DescriptorPool &operator=(DescriptorPool &&other) noexcept;

  VkDescriptorPool GetHandle() const;

  static VkDescriptorPool Create(std::span<const Walle::DescriptorPoolSize> sizes, Walle::DescriptorPoolCreateMask mask, uint32_t max_sets);

private:
  VkDescriptorPool descriptor_pool_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_DESCRIPTOR_POOL_H