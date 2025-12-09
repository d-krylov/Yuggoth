#ifndef YUGGOTH_HIERARCHY_WINDOW_H
#define YUGGOTH_HIERARCHY_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class HierarchyWindow : public EditorWindow {
public:
  void OnImGui();

protected:
  void AddEntity();
  void DrawNodes();

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_HIERARCHY_WINDOW_H