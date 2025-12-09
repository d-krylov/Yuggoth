#ifndef YUGGOTH_LINEAR_ALLOCATOR_H
#define YUGGOTH_LINEAR_ALLOCATOR_H

#include <cstdint>

namespace Yuggoth {

class LinearAllocator {
public:
  void Allocate(std::size_t size, std::size_t alignment);

  void Reset();

private:
  std::size_t capacity_{0};
  std::size_t offset_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_LINEAR_ALLOCATOR_H