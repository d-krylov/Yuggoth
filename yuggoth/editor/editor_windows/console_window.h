#ifndef YUGGOTH_CONSOLE_WINDOW_H
#define YUGGOTH_CONSOLE_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class ConsoleWindow : public EditorWindow {
public:
  void OnImGui();

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_CONSOLE_WINDOW_H