#ifndef YUGGOTH_IMGUI_LAYER_H
#define YUGGOTH_IMGUI_LAYER_H

#include "yuggoth/gui/window/key_events.h"
#include "yuggoth/gui/window/mouse_events.h"
#include "yuggoth/gui/window/window_events.h"

namespace Yuggoth {

class Window;

class ImGuiHost {
public:
  ImGuiHost(const Window *window);

  void OnEvent(Event &event);

  void NewFrame();

protected:
  bool OnKeyEvent(const KeyEvent &event);
  bool OnMouseButtonEvent(const MouseButtonEvent &event);
  bool OnMouseScrollEvent(const MouseScrollEvent &event);
  bool OnMouseMoveEvent(const MouseMoveEvent &event);
  bool OnCharEvent(const CharEvent &event);

  void SetStyle();
  void UpdateMouseData();
  void UpdateTime();

private:
  const Window *window_ = nullptr;
  double time_ = 0.0;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMGUI_LAYER_H