#ifndef YUGGOTH_INTRUSIVE_REFERENCE_COUNTER_H
#define YUGGOTH_INTRUSIVE_REFERENCE_COUNTER_H

#include "yuggoth/core/tools/include/counter.h"
#include <memory>

namespace Yuggoth {

template <typename T> class IntrusivePointer;

template <typename T, typename DELETER = std::default_delete<T>, CounterConcept COUNTER = SingleThreadCounter> class IntrusiveReferenceCounter {
public:
  using IntrusivePointerType = IntrusivePointer<T>;
  using ObjectType = T;
  using DeleterType = DELETER;
  using ReferenceCounter = COUNTER;

  IntrusiveReferenceCounter() = default;
  IntrusiveReferenceCounter(const IntrusiveReferenceCounter &) = delete;

  void operator=(const IntrusiveReferenceCounter &) = delete;

  void AddReference() {
    reference_counter_.Add();
  }

  void ReleaseReference() {
    if (reference_counter_.Sub()) DELETER()(static_cast<T *>(this));
  }

  uint32_t UseCount() const {
    return reference_counter_.GetCounter();
  }

  IntrusivePointer<T> ReferenceFromThis();

private:
  COUNTER reference_counter_;
};

} // namespace Yuggoth

#endif // YUGGOTH_INTRUSIVE_REFERENCE_COUNTER_H