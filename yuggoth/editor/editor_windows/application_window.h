#ifndef YUGGOTH_APPLICATION_WINDOW_H
#define YUGGOTH_APPLICATION_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class ApplicationWindow : public EditorWindow {
public:
  void OnImGui();

protected:
  void DrawMemoryStatistics();
  void DrawSystemStatistics();
  void DrawBufferAllocator();
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_WINDOW_H