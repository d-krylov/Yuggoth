#ifndef YUGGOTH_COMMAND_POOL_H
#define YUGGOTH_COMMAND_POOL_H

#include "yuggoth/graphics/core/graphics_context_types.h"

namespace Yuggoth {

class CommandPool {
public:
  CommandPool() = default;

  CommandPool(QueueType queue_type);

  ~CommandPool();

  CommandPool(const CommandPool &) = delete;
  CommandPool &operator=(const CommandPool &) = delete;

  CommandPool(CommandPool &&other) noexcept;
  CommandPool &operator=(CommandPool &&other) noexcept;

  static VkCommandPool CreateCommandPool(QueueType queue_type);

  void Reset();

  VkCommandPool GetHandle() const;
  QueueType GetQueueType() const;

private:
  VkCommandPool command_pool_{VK_NULL_HANDLE};
  QueueType queue_type_ = QueueType::GRAPHICS;
};

} // namespace Yuggoth

#endif // YUGGOTH_COMMAND_POOL_H