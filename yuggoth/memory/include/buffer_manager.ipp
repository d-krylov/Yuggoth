#ifndef YUGGOTH_BUFFER_MANAGER_IPP
#define YUGGOTH_BUFFER_MANAGER_IPP

namespace Yuggoth {

template <typename T>
void BufferManager::AddBufferAllocator(uint64_t buffer_size, BufferUsageMask buffer_usage, AllocationCreateMask allocation_mask) {
  BufferAllocatorSpecification specification;
  specification.allocation_mask_ = allocation_mask;
  specification.buffer_size_ = buffer_size;
  specification.buffer_stride_ = sizeof(T);
  specification.buffer_typeid_ = T::type_id;
  specification.buffer_usage_ = buffer_usage;
  allocators_.emplace(T::type_id, specification);
}

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_MANAGER_IPP