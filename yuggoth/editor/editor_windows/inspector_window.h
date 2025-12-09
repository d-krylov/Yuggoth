#ifndef YUGGOTH_INSPECTOR_WINDOW_H
#define YUGGOTH_INSPECTOR_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class InspectorWindow : public EditorWindow {
public:
  void OnImGui();

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_INSPECTOR_WINDOW_H