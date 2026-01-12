#ifndef YUGGOTH_DESCRIPTOR_SET_H
#define YUGGOTH_DESCRIPTOR_SET_H

#include "yuggoth/graphics_device/graphics_device.h"
#include <span>

namespace Yuggoth {

class DescriptorSet {
public:
  DescriptorSet() = default;

  DescriptorSet(VkDescriptorPool descriptor_pool, VkDescriptorSetLayout set_layout, uint32_t descriptors_count);

  ~DescriptorSet();

  DescriptorSet(const DescriptorSet &) = delete;
  DescriptorSet &operator=(const DescriptorSet &) = delete;

  DescriptorSet(DescriptorSet &&other) noexcept;
  DescriptorSet &operator=(DescriptorSet &&other) noexcept;

  void Update(std::span<const DescriptorImageInfo> images, uint32_t binding, DescriptorType descriptor_type, uint32_t start);

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