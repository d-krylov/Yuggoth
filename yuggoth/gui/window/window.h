#ifndef YUGGOTH_WINDOW_H
#define YUGGOTH_WINDOW_H

#include <string_view>
#include <functional>
#include "yuggoth/core/tools/include/core_types.h"

struct GLFWwindow;

namespace Yuggoth {

class Event;

using EventHandler = std::function<void(Event &)>;

class Window {
public:
  Window(int32_t width, int32_t height, std::string_view name);

  GLFWwindow *GetNativeWindow() const;

  ViewportSize GetWindowSize() const;
  ViewportSize GetFramebufferSize() const;
  CursorPosition GetCursorPosition() const;

  bool ShouldClose() const;

  int32_t GetKey(int32_t key) const;

  void Invoke(Event &event);
  void SetEventHandler(const EventHandler &handler);

private:
  GLFWwindow *native_window_{nullptr};
  EventHandler event_handler_;
};

} // namespace Yuggoth

#endif // YUGGOTH_WINDOW_H