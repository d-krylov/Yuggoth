#ifndef YUGGOTH_DESCRIPTOR_POOL_H
#define YUGGOTH_DESCRIPTOR_POOL_H

#include <walle/walle.h>
#include <yuggoth/core/tools/yuggoth_macros.h>
#include <span>

namespace Yuggoth {

class DescriptorPool {
public:
  DISABLE_COPY_SEMANTICS(DescriptorPool)

  DescriptorPool() = default;
  DescriptorPool(std::span<const Walle::DescriptorPoolSize> descriptor_pool_sizes, Walle::DescriptorPoolCreateMask mask, uint32_t max_sets);

  DescriptorPool(DescriptorPool &&other) noexcept;
  DescriptorPool &operator=(DescriptorPool &&other) noexcept;

  ~DescriptorPool();

  VkDescriptorPool GetHandle() const;

  static VkDescriptorPool Create(std::span<const Walle::DescriptorPoolSize> sizes, Walle::DescriptorPoolCreateMask mask, uint32_t max_sets);

private:
  VkDescriptorPool descriptor_pool_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_DESCRIPTOR_POOL_H