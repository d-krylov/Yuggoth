#include "command_pool.h"

namespace Yuggoth {

CommandPool::CommandPool(uint32_t queue_family_index) {
  VkCommandPoolCreateInfo command_pool_ci{};
  {
    command_pool_ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_ci.queueFamilyIndex = queue_family_index;
  }
  VK_CHECK(vkCreateCommandPool(GraphicsContext::Get()->GetDevice(), &command_pool_ci, 0, &command_pool));
}

CommandPool::~CommandPool() {
}

VkCommandPool CommandPool::GetHandle() const {
  return command_pool;
}

} // namespace Yuggoth