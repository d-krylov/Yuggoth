#ifndef YUGGOTH_RESOURCE_MANAGER_IPP
#define YUGGOTH_RESOURCE_MANAGER_IPP

#include <yuggoth/core/tools/tools.h>

namespace Yuggoth {

template <typename T> StaticBufferAllocator &ResourceManager::GetStaticBufferAllocator() {
  auto type_id = GetTypeId<T>();
  return static_buffers_.at(type_id);
}

template <typename T> void ResourceManager::RegisterStaticBufferAllocator(const BufferCreateInformation &buffer_ci) {
  auto type_id = GetTypeId<T>();
  StaticBufferAllocator static_buffer;
  static_buffer.stride_ = sizeof(T);
  static_buffer.typeid_ = type_id;
  static_buffer.buffer_ = Buffer(buffer_ci);
  static_buffer.allocator_ = VirtualAllocator(buffer_ci.buffer_size_);
  static_buffers_.emplace(type_id, std::move(static_buffer));
}

template <typename T> StaticBufferRange ResourceManager::UploadStaticBuffer(std::span<const T> data) {
  StaticBufferRange static_buffer_range;
  auto type_id = GetTypeId<T>();
  auto offset = UploadStaticBuffer(type_id, std::as_bytes(data));
  static_buffer_range.typeid_ = type_id;
  static_buffer_range.stride_ = sizeof(T);
  static_buffer_range.offset_ = offset / static_buffer_range.stride_;
  static_buffer_range.count_ = data.size();
  static_buffer_range.buffer_ = static_buffers_.at(type_id).buffer_.GetHandle();
  return static_buffer_range;
}

} // namespace Yuggoth

#endif // YUGGOTH_RESOURCE_MANAGER_IPP
