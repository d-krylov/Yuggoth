#ifndef YUGGOTH_EDITOR_WINDOW_H
#define YUGGOTH_EDITOR_WINDOW_H

#include "yuggoth/application/application_context.h"
#include <string>
#include <string_view>

namespace Yuggoth {

class Editor;

class EditorWindow {
public:
  void Begin();

  void End();

  Editor *GetEditor() const;
  EditorContext *GetEditorContext() const;

  void SetEditor(Editor *editor);
  void SetEditorContext(EditorContext *editor_context);

  std::string_view GetName() const;

private:
  Editor *editor_ = nullptr;
  EditorContext *editor_context_ = nullptr;
  std::string name_;
  bool show_window_;
};

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_WINDOW_H