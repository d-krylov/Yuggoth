#include "descriptor_set.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include <vector>

namespace Yuggoth {

VkDescriptorSetLayout DescriptorSet::CreateDescriptorSetLayout(std::span<const Walle::DescriptorSetLayoutBinding> descriptor_set_bindings,
                                                               Walle::DescriptorSetLayoutCreateMask descriptor_set_create_mask) {

  auto binding_count = descriptor_set_bindings.size();
  std::vector<Walle::DescriptorBindingMask> binding_masks(binding_count);

  for (auto i = 0; i < binding_count; i++) {
    auto is_array = descriptor_set_bindings[i].descriptorCount > 1;
    auto is_runtime_array = descriptor_set_bindings[i].descriptorCount == 0;
    auto is_end = (i + 1 == binding_count);

    if (is_runtime_array) {
      binding_masks[i] |= Walle::DescriptorBindingMaskBits::E_PARTIALLY_BOUND_BIT;
    }

    if (descriptor_set_create_mask.HasBits(Walle::DescriptorSetLayoutCreateMaskBits::E_UPDATE_AFTER_BIND_POOL_BIT)) {
      binding_masks[i] |= Walle::DescriptorBindingMaskBits::E_UPDATE_AFTER_BIND_BIT;
    }

    if (is_end && is_runtime_array) {
      binding_masks[i] |= Walle::DescriptorBindingMaskBits::E_VARIABLE_DESCRIPTOR_COUNT_BIT;
    }
  }

  Walle::DescriptorSetLayoutBindingFlagsCreateInfo set_binding_flags_ci;
  set_binding_flags_ci.bindingCount = binding_masks.size();
  set_binding_flags_ci.pBindingFlags = binding_masks.data();

  Walle::DescriptorSetLayoutCreateInfo descriptor_set_layout_ci;
  descriptor_set_layout_ci.bindingCount = descriptor_set_bindings.size();
  descriptor_set_layout_ci.pBindings = descriptor_set_bindings.data();
  descriptor_set_layout_ci.flags = descriptor_set_create_mask;
  descriptor_set_layout_ci.pNext = &set_binding_flags_ci;

  VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
  VK_CHECK(vkCreateDescriptorSetLayout(GraphicsContext::Get()->GetDevice(), descriptor_set_layout_ci, 0, &descriptor_set_layout));
  return descriptor_set_layout;
}

DescriptorSet::DescriptorSet(VkDescriptorPool descriptor_pool, VkDescriptorSetLayout set_layout, uint32_t descriptors_count) {
  descriptor_set_ = AllocateDescriptorSet(descriptor_pool, set_layout, descriptors_count);
}

DescriptorSet::~DescriptorSet() {
}

DescriptorSet::DescriptorSet(DescriptorSet &&other) noexcept {
  descriptor_set_ = std::exchange(other.descriptor_set_, VK_NULL_HANDLE);
  descriptor_set_layout_ = std::exchange(other.descriptor_set_layout_, VK_NULL_HANDLE);
}

DescriptorSet &DescriptorSet::operator=(DescriptorSet &&other) noexcept {
  std::swap(descriptor_set_, other.descriptor_set_);
  return *this;
}

VkDescriptorSet AllocateBindlessSet(VkDescriptorPool descriptor_pool, uint32_t descriptors_count) {
  auto number_of_descriptors = descriptors_count;

  Walle::DescriptorSetVariableDescriptorCountAllocateInfo descriptor_set_count_ai;
  descriptor_set_count_ai.descriptorSetCount = 1;
  descriptor_set_count_ai.pDescriptorCounts = &number_of_descriptors;

  Walle::DescriptorSetAllocateInfo descriptor_set_ai;
  descriptor_set_ai.pNext = &descriptor_set_count_ai;
  descriptor_set_ai.descriptorPool = descriptor_pool;
  descriptor_set_ai.descriptorSetCount = 1;
  // descriptor_set_ai.pSetLayouts = &set_layout;

  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
  VK_CHECK(vkAllocateDescriptorSets(GraphicsContext::Get()->GetDevice(), descriptor_set_ai, &descriptor_set));
  return descriptor_set;
}

VkDescriptorSet DescriptorSet::AllocateDescriptorSet(VkDescriptorPool descriptor_pool, VkDescriptorSetLayout set_layout, uint32_t descriptors_count) {
  Walle::DescriptorSetVariableDescriptorCountAllocateInfo descriptor_set_count_ai;
  descriptor_set_count_ai.descriptorSetCount = 1;
  descriptor_set_count_ai.pDescriptorCounts = &descriptors_count;

  Walle::DescriptorSetAllocateInfo descriptor_set_ai;
  descriptor_set_ai.pNext = &descriptor_set_count_ai;
  descriptor_set_ai.descriptorPool = descriptor_pool;
  descriptor_set_ai.descriptorSetCount = 1;
  descriptor_set_ai.pSetLayouts = &set_layout;

  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
  VK_CHECK(vkAllocateDescriptorSets(GraphicsContext::Get()->GetDevice(), descriptor_set_ai, &descriptor_set));
  return descriptor_set;
}

void DescriptorSet::Update(std::span<const Walle::DescriptorImageInfo> images, uint32_t binding, Walle::DescriptorType descriptor_type, uint32_t start) {
  Walle::WriteDescriptorSet write_descriptor_set;
  write_descriptor_set.dstSet = descriptor_set_;
  write_descriptor_set.dstBinding = binding;
  write_descriptor_set.dstArrayElement = start;
  write_descriptor_set.descriptorType = descriptor_type;
  write_descriptor_set.descriptorCount = images.size();
  write_descriptor_set.pImageInfo = images.data();
  vkUpdateDescriptorSets(GraphicsContext::Get()->GetDevice(), 1, write_descriptor_set, 0, nullptr);
}

VkDescriptorSet DescriptorSet::GetHandle() const {
  return descriptor_set_;
}

} // namespace Yuggoth