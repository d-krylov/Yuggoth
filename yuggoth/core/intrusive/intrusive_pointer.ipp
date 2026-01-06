#ifndef YUGGOTH_INTRUSIVE_POINTER_IPP
#define YUGGOTH_INTRUSIVE_POINTER_IPP

#include <utility>

namespace Yuggoth {

template <typename T> uint32_t IntrusivePointer<T>::use_count() const {
  return static_cast<ReferenceBase *>(object_pointer_)->UseCount();
}

template <typename T> T &IntrusivePointer<T>::operator*() const {
  return *object_pointer_;
}

template <typename T> T *IntrusivePointer<T>::operator->() const {
  return object_pointer_;
}

template <typename T> T *IntrusivePointer<T>::get() const {
  return object_pointer_;
}

template <typename T> void IntrusivePointer<T>::reset() {
  if (object_pointer_) {
    static_cast<ReferenceBase *>(object_pointer_)->ReleaseReference();
  }
  object_pointer_ = nullptr;
}

template <typename T> template <typename U> IntrusivePointer<T> &IntrusivePointer<T>::operator=(const IntrusivePointer<U> &other) {
  static_assert(std::derived_from<U, T>, "Cannot safely assign downcasted intrusive pointers.");
  reset();
  object_pointer_ = static_cast<T *>(other.object_pointer_);
  if (object_pointer_) {
    static_cast<ReferenceBase *>(object_pointer_)->AddReference();
  }
  return *this;
}

template <typename T> IntrusivePointer<T> &IntrusivePointer<T>::operator=(const IntrusivePointer<T> &other) {
  if (this != &other) {
    reset();
    object_pointer_ = other.object_pointer_;
    if (object_pointer_) {
      static_cast<ReferenceBase *>(object_pointer_)->AddReference();
    }
  }
  return *this;
}

template <typename T> template <typename U> IntrusivePointer<T> &IntrusivePointer<T>::operator=(IntrusivePointer<U> &&other) noexcept {
  reset();
  object_pointer_ = std::exchange(other.object_pointer_, nullptr);
  return *this;
}

template <typename T> IntrusivePointer<T> &IntrusivePointer<T>::operator=(IntrusivePointer<T> &&other) noexcept {
  if (this != &other) {
    reset();
    object_pointer_ = std::exchange(other.object_pointer_, nullptr);
  }
  return *this;
}

template <typename T> T *IntrusivePointer<T>::release() {
  return std::exchange(object_pointer_, nullptr);
}

template <typename T, typename... ARGUMENTS> IntrusivePointer<T> MakeIntrusivePointer(ARGUMENTS &&...arguments) {
  return IntrusivePointer<T>(new T(std::forward<ARGUMENTS>(arguments)...));
}

template <typename T, typename U> IntrusivePointer<T> static_pointer_cast(const IntrusivePointer<U> &intrusive_pointer) {
  return IntrusivePointer(static_cast<T *>(intrusive_pointer.get()));
}

} // namespace Yuggoth

#endif // YUGGOTH_INTRUSIVE_POINTER_IPP