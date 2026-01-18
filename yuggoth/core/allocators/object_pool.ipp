#ifndef YUGGOTH_OBJECT_POOL_IPP
#define YUGGOTH_OBJECT_POOL_IPP

namespace Yuggoth {

template <typename T> template <typename... Arguments> T *ObjectPool<T>::allocate(Arguments &&...arguments) {
  if (free_.empty()) {
    auto status = grow();
    if (status == false) {
      return nullptr;
    }
  }

  auto free_pointer = free_.back();
  free_.pop_back();
  return std::construct_at(free_pointer, std::forward<Arguments>(arguments)...);
}

template <typename T> bool ObjectPool<T>::grow() {
  auto number_objects = 64u << memory_.size();
  auto alignment = std::max<std::size_t>(alignof(T), 64);

  auto void_memory = std::aligned_alloc(alignment, number_objects * sizeof(T));
  auto type_memory = static_cast<T *>(void_memory);

  if (void_memory == nullptr) {
    return false;
  }

  for (auto i = 0; i < number_objects; ++i) {
    free_.push_back(&type_memory[i]);
  }

  memory_.emplace_back(type_memory);
  return true;
}

template <typename T> void ObjectPool<T>::free(T *object) {
  std::destroy_at(object);
  free_.push_back(object);
}

template <typename T> void ObjectPool<T>::clear() {
  free_.clear();
  memory_.clear();
}

template <typename T> template <typename... Arguments> T *ThreadSafeObjectPool<T>::allocate(Arguments &&...arguments) {
  std::lock_guard<std::mutex> holder{mutex_};
  return ObjectPool<T>::allocate(std::forward<Arguments>(arguments)...);
}

template <typename T> void ThreadSafeObjectPool<T>::free(T *object) {
  std::destroy_at(object);
  std::lock_guard<std::mutex> holder{mutex_};
  this->free_.push_back(object);
}

template <typename T> void ThreadSafeObjectPool<T>::clear() {
  std::lock_guard<std::mutex> holder{mutex_};
  ObjectPool<T>::clear();
}

} // namespace Yuggoth

#endif // YUGGOTH_OBJECT_POOL_IPP