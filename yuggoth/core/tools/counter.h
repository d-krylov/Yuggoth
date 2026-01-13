#ifndef YUGGOTH_COUNTER_H
#define YUGGOTH_COUNTER_H

#include <concepts>
#include <cstdint>
#include <atomic>

namespace Yuggoth {

template <typename T>
concept CounterConcept = requires(T counter) {
  { counter.Add() } -> std::same_as<void>;
  { counter.Sub() } -> std::convertible_to<bool>;
  { counter.GetCounter() } -> std::same_as<uint32_t>;
};

class SingleThreadCounter {
public:
  SingleThreadCounter(uint32_t start = 0) : count_(start) {
  }

  void Add() {
    count_++;
  }

  bool Sub() {
    return --count_ == 0;
  }

  uint32_t GetCounter() const {
    return count_;
  }

private:
  uint32_t count_ = 0;
};

class MultiThreadCounter {
public:
  MultiThreadCounter(uint32_t start = 0) {
    count_.store(start, std::memory_order_relaxed);
  }

  void Add() {
    count_.fetch_add(1, std::memory_order_relaxed);
  }

  bool Sub() {
    auto result = count_.fetch_sub(1, std::memory_order_acq_rel);
    return result == 1;
  }

  uint32_t GetCounter() const {
    return count_.load(std::memory_order_acquire);
  }

private:
  std::atomic_uint32_t count_;
};

} // namespace Yuggoth

#endif // YUGGOTH_COUNTER_H