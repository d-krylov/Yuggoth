#ifndef YUGGOTH_MACROS_H
#define YUGGOTH_MACROS_H

namespace Yuggoth {

#define DISABLE_COPY_SEMANTICS(class_name)                                                                                                                     \
  class_name(const class_name &) = delete;                                                                                                                     \
  class_name &operator=(const class_name &) = delete;

#define DISABLE_MOVE_SEMANTICS(class_name)                                                                                                                     \
  class_name(class_name &&) = delete;                                                                                                                          \
  class_name &operator=(class_name &&) = delete;

} // namespace Yuggoth

#endif // YUGGOTH_MACROS_H