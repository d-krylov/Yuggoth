#ifndef YUGGOTH_DESCRIPTOR_SET_H
#define YUGGOTH_DESCRIPTOR_SET_H

#include <walle/walle.h>
#include <yuggoth/core/tools/yuggoth_macros.h>
#include <span>

namespace Yuggoth {

class DescriptorSet {
public:
  DISABLE_COPY_SEMANTICS(DescriptorSet)

  DescriptorSet() = default;
  DescriptorSet(VkDescriptorPool descriptor_pool, VkDescriptorSetLayout set_layout, uint32_t descriptors_count);

  ~DescriptorSet();

  DescriptorSet(DescriptorSet &&other) noexcept;
  DescriptorSet &operator=(DescriptorSet &&other) noexcept;

  void Update(std::span<const Walle::DescriptorImageInfo> images, uint32_t binding, Walle::DescriptorType descriptor_type, uint32_t start);

  VkDescriptorSet GetHandle() const;

  static VkDescriptorSet AllocateDescriptorSet(VkDescriptorPool descriptor_pool, VkDescriptorSetLayout set_layout, uint32_t descriptors_count);
  static VkDescriptorSetLayout CreateDescriptorSetLayout(std::span<const Walle::DescriptorSetLayoutBinding> descriptor_set_bindings,
                                                         Walle::DescriptorSetLayoutCreateMask descriptor_set_create_mask);

private:
  VkDescriptorSet descriptor_set_{VK_NULL_HANDLE};
  VkDescriptorSetLayout descriptor_set_layout_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_DESCRIPTOR_SET_H