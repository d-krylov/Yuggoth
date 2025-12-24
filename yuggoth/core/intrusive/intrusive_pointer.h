#ifndef YUGGOTH_INTRUSIVE_POINTER_H
#define YUGGOTH_INTRUSIVE_POINTER_H

#include "intrusive_reference_counter.h"
#include <print>

namespace Yuggoth {

template <typename T> class IntrusivePointer {
public:
  using ReferenceBase = IntrusiveReferenceCounter<typename T::ObjectType, typename T::DeleterType, typename T::ReferenceCounter>;

  template <typename U> friend class IntrusivePointer;

  IntrusivePointer() = default;

  explicit IntrusivePointer(T *object_pointer, bool add_reference = true) : object_pointer_(object_pointer) {
    if (object_pointer != nullptr && add_reference) static_cast<ReferenceBase *>(object_pointer_)->AddReference();
  }

  template <typename U>
    requires std::derived_from<U, T>
  IntrusivePointer(const IntrusivePointer<U> &other) {
    *this = other;
  }

  template <typename U>
    requires std::derived_from<U, T>
  IntrusivePointer(IntrusivePointer<U> &&other) noexcept {
    *this = std::move(other);
  }

  ~IntrusivePointer() {
    reset();
  }

  explicit operator bool() const {
    return object_pointer_ != nullptr;
  }

  T &operator*();
  T *operator->();
  T *get();

  const T &operator*() const;
  const T *operator->() const;
  const T *get() const;

  void reset();

  T *release();

  uint32_t use_count() const;

  bool operator==(const IntrusivePointer &other) const = default;

  template <typename U> IntrusivePointer<T> &operator=(const IntrusivePointer<U> &other);

  template <typename U> IntrusivePointer<T> &operator=(IntrusivePointer<U> &&other) noexcept;

  IntrusivePointer<T> &operator=(const IntrusivePointer<T> &other);

  IntrusivePointer<T> &operator=(IntrusivePointer<T> &&other) noexcept;

private:
  T *object_pointer_ = nullptr;
};

} // namespace Yuggoth

#include "intrusive_pointer.ipp"

#endif // YUGGOTH_INTRUSIVE_POINTER_H