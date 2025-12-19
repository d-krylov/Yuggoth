#ifndef YUGGOTH_FREE_LIST_ALLOCATOR_H
#define YUGGOTH_FREE_LIST_ALLOCATOR_H

#include <set>
#include <cstdint>

namespace Yuggoth {

struct FreeRange {
  uint32_t offset;
  uint32_t size;

  bool operator<(const FreeRange &other) const {
    return offset < other.offset;
  }
};

class FreeListAllocator {
public:
  FreeListAllocator();

  std::size_t Allocate(std::size_t count);

  void Free();

private:
  std::size_t element_size_;
  std::set<FreeRange> free_ranges_;
};

} // namespace Yuggoth

#endif // YUGGOTH_FREE_LIST_ALLOCATOR_H