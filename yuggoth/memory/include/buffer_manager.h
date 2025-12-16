#ifndef YUGGOTH_BUFFER_MANAGER_H
#define YUGGOTH_BUFFER_MANAGER_H

#include "buffer_allocator.h"
#include <vector>

namespace Yuggoth {

struct BufferRangeInformation {
  uint32_t stride_;
  uint32_t offset_;
  uint32_t size_;
  uint64_t uuid_;
};

class BufferManager {
public:
  BufferManager();

protected:
  void Create();

private:
  BufferAllocator staging_allocator_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_MANAGER_H