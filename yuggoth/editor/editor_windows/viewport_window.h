#ifndef YUGGOTH_VIEWPORT_WINDOW_H
#define YUGGOTH_VIEWPORT_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"
#include <yuggoth/core/tools/tools.h>

namespace Yuggoth {

class ViewportWindow : public EditorWindow {
public:
  void OnImGui();

  vector2i GetViewportSize() const;

private:
  vector2i viewport_size_ = vector2i(0, 0);
};

} // namespace Yuggoth

#endif // YUGGOTH_VIEWPORT_WINDOW_H