#ifndef YUGGOTH_RENDERER_WINDOW_H
#define YUGGOTH_RENDERER_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class RendererWindow : public EditorWindow {
public:
  void OnImGui();
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_WINDOW_H