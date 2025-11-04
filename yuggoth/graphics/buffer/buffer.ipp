#ifndef YUGGOTH_BUFFER_IPP
#define YUGGOTH_BUFFER_IPP

namespace Yuggoth {

template <typename T>
std::span<T> Buffer::GetMappedAs() {
  auto size = buffer_size_ / sizeof(T);
  auto data = reinterpret_cast<T *>(mapped_memory_);
  return std::span<T>(data, size);
}

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_IPP