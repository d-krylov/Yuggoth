#include "command_pool.h"

namespace Yuggoth {

CommandPool::CommandPool(uint32_t queue_family_index) {
  command_pool_ = GraphicsContext::Get()->CreateCommandPool(queue_family_index);
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

VkCommandPool CommandPool::GetHandle() const {
  return command_pool_;
}

} // namespace Yuggoth