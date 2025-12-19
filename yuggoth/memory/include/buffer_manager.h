#ifndef YUGGOTH_BUFFER_MANAGER_H
#define YUGGOTH_BUFFER_MANAGER_H

#include "buffer_allocator.h"
#include <vector>

namespace Yuggoth {

class BufferManager {
public:
  BufferManager();

  BufferAllocator &GetVertexAllocator();
  BufferAllocator &GetIndexAllocator();

protected:
  void Create();

private:
  BufferAllocator vertex_allocator_;
  BufferAllocator index_allocator_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_MANAGER_H