#include "descriptor_pool.h"

namespace Yuggoth {

DescriptorPool::DescriptorPool(std::span<const DescriptorPoolSize> descriptor_pool_sizes, DescriptorPoolCreateMask mask, uint32_t max_sets) {
  descriptor_pool_ = CreateDescriptorPool(descriptor_pool_sizes, mask, max_sets);
}

DescriptorPool::~DescriptorPool() {
}

VkDescriptorPool DescriptorPool::GetHandle() const {
  return descriptor_pool_;
}

DescriptorPool::DescriptorPool(DescriptorPool &&other) noexcept {
  descriptor_pool_ = std::exchange(other.descriptor_pool_, VK_NULL_HANDLE);
}

DescriptorPool &DescriptorPool::operator=(DescriptorPool &&other) noexcept {
  std::swap(descriptor_pool_, other.descriptor_pool_);
  return *this;
}

VkDescriptorPool DescriptorPool::CreateDescriptorPool(std::span<const DescriptorPoolSize> sizes, DescriptorPoolCreateMask mask, uint32_t max_sets) {
  DescriptorPoolCreateInfo descriptor_pool_ci;
  descriptor_pool_ci.flags = mask;
  descriptor_pool_ci.maxSets = max_sets;
  descriptor_pool_ci.poolSizeCount = sizes.size();
  descriptor_pool_ci.pPoolSizes = sizes.data();

  VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
  VK_CHECK(vkCreateDescriptorPool(GraphicsContext::Get()->GetDevice(), descriptor_pool_ci, 0, &descriptor_pool));
  return descriptor_pool;
}

} // namespace Yuggoth