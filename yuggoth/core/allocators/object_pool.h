#ifndef YUGGOTH_OBJECT_POOL_H
#define YUGGOTH_OBJECT_POOL_H

#include <vector>
#include <memory>
#include <mutex>

namespace Yuggoth {

template <typename T> class ObjectPool {
public:
  template <typename... Arguments> T *allocate(Arguments &&...arguments);

  void free(T *object);
  void clear();

protected:
  bool grow();

private:
  struct MallocDeleter {
    void operator()(T *pointer) const noexcept {
      std::free(pointer);
    }
  };

private:
  std::vector<std::unique_ptr<T, MallocDeleter>> memory_;
  std::vector<T *> free_;
};

template <typename T> class ThreadSafeObjectPool : private ObjectPool<T> {
public:
  template <typename... Arguments> T *allocate(Arguments &&...arguments);

  void free(T *ptr);
  void clear();

private:
  std::mutex mutex_;
};

} // namespace Yuggoth

#include "object_pool.ipp"

#endif // YUGGOTH_OBJECT_POOL_H