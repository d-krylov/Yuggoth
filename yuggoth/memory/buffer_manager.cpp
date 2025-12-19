#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/asset/include/mesh.h"
#include "yuggoth/core/tools/include/core.h"

namespace Yuggoth {

BufferManager::BufferManager() {
  Create();
}

void BufferManager::Create() {
  vertex_allocator_ = BufferAllocator(Buffer(100_MiB, BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, Buffer::CPU), sizeof(Vertex));
  index_allocator_ = BufferAllocator(Buffer(100_MiB, BufferUsageMaskBits::E_INDEX_BUFFER_BIT, Buffer::CPU), sizeof(uint32_t));
}

BufferAllocator &BufferManager::GetVertexAllocator() {
  return vertex_allocator_;
}

BufferAllocator &BufferManager::GetIndexAllocator() {
  return index_allocator_;
}

} // namespace Yuggoth