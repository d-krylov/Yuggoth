#include "linear_allocator.h"
#include "yuggoth/core/tools/include/core.h"

namespace Yuggoth {

void LinearAllocator::Allocate(std::size_t size, std::size_t alignment) {

  auto aligned = AlignUp(offset_, alignment);

  if (aligned + size > capacity_) {
  }

  offset_ = aligned + size;
}

void LinearAllocator::Reset() {
  offset_ = 0;
}

} // namespace Yuggoth