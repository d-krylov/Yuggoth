#ifndef YUGGOTH_EDITOR_WINDOW_H
#define YUGGOTH_EDITOR_WINDOW_H

#include <string>
#include <string_view>

namespace Yuggoth {

class Editor;

class EditorWindow {
public:
  void Begin();

  void End();

  Editor *GetEditor() const;

  void SetEditor(Editor *editor);

  std::string_view GetName() const;

private:
  Editor *editor_{nullptr};
  std::string name_;
  bool show_window_;
};

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_WINDOW_H