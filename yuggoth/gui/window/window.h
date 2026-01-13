#ifndef YUGGOTH_WINDOW_H
#define YUGGOTH_WINDOW_H

#include <string_view>
#include <functional>
#include "yuggoth/core/tools/core_types.h"
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Yuggoth {

class Event;

using EventHandler = std::function<void(Event &)>;

class Window {
public:
  Window(int32_t width, int32_t height, std::string_view name);

  GLFWwindow *GetNativeWindow() const;

  vector2i GetWindowSize() const;
  vector2i GetFramebufferSize() const;
  vector2d GetCursorPosition() const;

  bool ShouldClose() const;

  int32_t GetKey(int32_t key) const;

  void Invoke(Event &event);
  void SetEventHandler(const EventHandler &handler);

  VkSurfaceKHR CreateSurface(VkInstance instance);
  static std::vector<const char *> GetRequiredInstanceExtensions();

private:
  GLFWwindow *native_window_{nullptr};
  EventHandler event_handler_;
};

} // namespace Yuggoth

#endif // YUGGOTH_WINDOW_H