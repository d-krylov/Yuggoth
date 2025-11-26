#ifndef YUGGOTH_BUFFER_IPP
#define YUGGOTH_BUFFER_IPP

namespace Yuggoth {

template <typename T> std::span<T> Buffer::GetMappedAs() {
  auto out_data = reinterpret_cast<T *>(mapped_memory_);
  auto out_size = buffer_size_ / sizeof(T);
  return std::span(out_data, out_size);
}

template <typename T> void Buffer::SetData(std::span<const T> data, std::size_t byte_offset) {
  GraphicsAllocator::Get()->CopyMemoryToAllocation(std::as_bytes(data), allocation_, byte_offset);
}

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_IPP