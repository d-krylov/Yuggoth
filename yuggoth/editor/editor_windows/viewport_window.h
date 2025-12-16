#ifndef YUGGOTH_VIEWPORT_WINDOW_H
#define YUGGOTH_VIEWPORT_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"
#include "yuggoth/core/tools/include/core_types.h"

namespace Yuggoth {

class ViewportWindow : public EditorWindow {
public:
  void OnImGui();

  ViewportSize GetViewportSize() const;

private:
  ViewportSize viewport_size_;
};

} // namespace Yuggoth

#endif // YUGGOTH_VIEWPORT_WINDOW_H