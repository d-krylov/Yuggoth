#ifndef YUGGOTH_IMGUI_LAYER_H
#define YUGGOTH_IMGUI_LAYER_H

#include "yuggoth/gui/window/key_events.h"
#include "yuggoth/gui/window/mouse_events.h"
#include "yuggoth/gui/window/window_events.h"

namespace Yuggoth {

class Window;

class ImGuiLayer {
public:
  ImGuiLayer(Window *window);

protected:
  bool OnKeyEvent(const KeyEvent &event);
  bool OnMouseButtonEvent(const MouseButtonEvent &event);
  bool OnMouseScrollEvent(const MouseScrollEvent &event);
  bool OnMouseMoveEvent(const MouseMoveEvent &event);
  bool OnCharEvent(const CharEvent &event);

private:
  Window *window_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMGUI_LAYER_H