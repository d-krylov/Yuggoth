#ifndef YUGGOTH_BUFFER_MANAGER_IPP
#define YUGGOTH_BUFFER_MANAGER_IPP

namespace Yuggoth {

template <typename T> void BufferManager::AddBufferAllocator(const BufferCreateInformation &buffer_ci) {
  BufferAllocatorSpecification specification;
  specification.buffer_ci_ = buffer_ci;
  specification.buffer_stride_ = sizeof(T);
  specification.buffer_typeid_ = T::type_id;
  allocators_.emplace(T::type_id, specification);
}

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_MANAGER_IPP