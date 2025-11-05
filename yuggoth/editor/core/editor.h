#ifndef YUGGOTH_EDITOR_H
#define YUGGOTH_EDITOR_H

#include "yuggoth/editor/editor_windows/editor_windows.h"

namespace Yuggoth {

class Editor {
public:
  Editor();

  void OnImGui();

protected:
  void DrawMainMenu();

private:
  ViewportWindow viewport_window_;
  HierarchyWindow hierarchy_window_;
};

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_H