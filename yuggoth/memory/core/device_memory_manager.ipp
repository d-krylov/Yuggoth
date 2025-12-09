#ifndef YUGGOTH_DEVICE_MEMORY_MANAGER_IPP
#define YUGGOTH_DEVICE_MEMORY_MANAGER_IPP

namespace Yuggoth {

template <typename T> void DeviceMemoryManager::SetScratchData(std::span<const T> data) {
  auto size = data.size_bytes();
  auto offset = scratch_allocator_.Allocate(size, 4);
  scratch_buffer_.SetData<T>(data);
}

} // namespace Yuggoth

#endif // YUGGOTH_DEVICE_MEMORY_MANAGER_IPP