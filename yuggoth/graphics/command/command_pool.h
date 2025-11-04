#ifndef YUGGOTH_COMMAND_POOL_H
#define YUGGOTH_COMMAND_POOL_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class CommandPool {
public:
  CommandPool(uint32_t queue_family_index);

  ~CommandPool();

  CommandPool(const CommandPool &) = delete;
  CommandPool &operator=(const CommandPool &) = delete;

  CommandPool(CommandPool &&other) noexcept;
  CommandPool &operator=(CommandPool &&other) noexcept;

  VkCommandPool GetHandle() const;

private:
  VkCommandPool command_pool_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_COMMAND_POOL_H