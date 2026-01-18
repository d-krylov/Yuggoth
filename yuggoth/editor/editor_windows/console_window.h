#ifndef YUGGOTH_CONSOLE_WINDOW_H
#define YUGGOTH_CONSOLE_WINDOW_H

#include <yuggoth/editor/core/editor_window.h>
#include <string>
#include <vector>

namespace Yuggoth {

struct ConsoleWindowMessage {
  std::string message_;
};

class ConsoleWindow : public EditorWindow {
public:
  ConsoleWindow();

  void OnImGui();

  void PushMessage(const ConsoleWindowMessage &message);

private:
  std::vector<ConsoleWindowMessage> messages_;
};

} // namespace Yuggoth

#endif // YUGGOTH_CONSOLE_WINDOW_H