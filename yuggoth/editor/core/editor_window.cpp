#include "editor_window.h"
#include "editor.h"
#include "imgui.h"

namespace Yuggoth {

void EditorWindow::Begin() {
}

void EditorWindow::End() {
}

Editor *EditorWindow::GetEditor() const {
  return editor_;
}

void EditorWindow::SetEditor(Editor *editor) {
  editor_ = editor;
}

std::string_view EditorWindow::GetName() const {
  return name_;
}

} // namespace Yuggoth