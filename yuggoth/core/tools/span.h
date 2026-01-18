#ifndef YUGGOTH_SPAN_H
#define YUGGOTH_SPAN_H

#include <span>
#include <type_traits>
#include "yuggoth_asserts.h"

namespace Yuggoth {

template <class FROM, class TO> std::span<TO> CastSpan(std::span<FROM> source) {
  static_assert(std::is_trivially_copyable_v<FROM>);
  static_assert(std::is_trivially_copyable_v<TO>);

  auto from_size = sizeof(FROM);
  auto to_size = sizeof(TO);

  auto size_bytes = source.size_bytes();

  auto new_size = size_bytes / to_size;
  auto new_rest = size_bytes % to_size;

  CORE_ASSERT(new_rest == 0, "The spans are not compatible");

  auto raw_cast = reinterpret_cast<TO *>(source.data());

  return std::span<TO>(raw_cast, new_size);
}

} // namespace Yuggoth

#endif // YUGGOTH_SPAN_H