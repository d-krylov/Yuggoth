#include "command_pool.h"

namespace Yuggoth {

CommandPool::CommandPool(uint32_t queue_family_index) {
  command_pool_ = CommandPool::CreateCommandPool(queue_family_index);
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(GraphicsContext::Get()->GetDevice(), command_pool_, nullptr);
}

CommandPool::CommandPool(CommandPool &&other) noexcept {
  command_pool_ = std::exchange(other.command_pool_, VK_NULL_HANDLE);
}

CommandPool &CommandPool::operator=(CommandPool &&other) noexcept {
  std::swap(command_pool_, other.command_pool_);
  return *this;
}

VkCommandPool CommandPool::CreateCommandPool(uint32_t queue_family_index) {
  CommandPoolCreateInfo command_pool_ci;
  command_pool_ci.flags = CommandPoolCreateMaskBits::E_RESET_COMMAND_BUFFER_BIT;
  command_pool_ci.queueFamilyIndex = queue_family_index;

  VkCommandPool command_pool = VK_NULL_HANDLE;
  VK_CHECK(vkCreateCommandPool(GraphicsContext::Get()->GetDevice(), command_pool_ci, 0, &command_pool));
  return command_pool;
}

VkCommandPool CommandPool::GetHandle() const {
  return command_pool_;
}

void CommandPool::Reset() {
  vkResetCommandPool(GraphicsContext::Get()->GetDevice(), command_pool_, 0);
}

} // namespace Yuggoth