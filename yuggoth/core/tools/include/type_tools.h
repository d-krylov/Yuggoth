#ifndef YUGGOTH_TYPE_TOOLS_H
#define YUGGOTH_TYPE_TOOLS_H

#include <tuple>
#include <vector>

namespace Yuggoth {

template <typename T> struct FunctionTraits;

template <typename R, typename... A> struct FunctionTraits<R (*)(A...)> {
  using arguments_t = std::tuple<A...>;
};

template <typename R, typename... A> struct FunctionTraits<R(A...)> {
  using arguments_t = std::tuple<A...>;
};

template <typename T, typename FUNCTION, typename... ARGUMENTS> inline auto Enumerate(FUNCTION &&enumerate_function, ARGUMENTS &&...arguments) {
  using A = std::remove_pointer_t<decltype(&enumerate_function)>;
  using R = FunctionTraits<A>::arguments_t;
  using V = std::tuple_element_t<std::tuple_size_v<R> - 1, R>;
  auto count{0u};
  std::forward<FUNCTION>(enumerate_function)(std::forward<ARGUMENTS>(arguments)..., &count, nullptr);
  std::vector<T> ret(count);
  std::forward<FUNCTION>(enumerate_function)(std::forward<ARGUMENTS>(arguments)..., &count, reinterpret_cast<V>(ret.data()));
  return ret;
}

} // namespace Yuggoth

#endif // YUGGOTH_TYPE_TOOLS_H