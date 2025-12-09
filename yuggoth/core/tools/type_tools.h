#ifndef YUGGOTH_TYPE_TOOLS_H
#define YUGGOTH_TYPE_TOOLS_H

#include <tuple>

namespace Yuggoth {

template <typename T> struct FunctionTraits;

template <typename R, typename... A> struct FunctionTraits<R (*)(A...)> {
  using arguments_t = std::tuple<A...>;
};

template <typename R, typename... A> struct FunctionTraits<R(A...)> {
  using arguments_t = std::tuple<A...>;
};

} // namespace Yuggoth

#endif // YUGGOTH_TYPE_TOOLS_H