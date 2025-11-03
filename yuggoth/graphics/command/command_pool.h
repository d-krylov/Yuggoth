#ifndef YUGGOTH_COMMAND_POOL_H
#define YUGGOTH_COMMAND_POOL_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class CommandPool {
public:
  CommandPool(uint32_t queue_family_index);

  ~CommandPool();

  VkCommandPool GetHandle() const;

private:
  VkCommandPool command_pool{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_COMMAND_POOL_H