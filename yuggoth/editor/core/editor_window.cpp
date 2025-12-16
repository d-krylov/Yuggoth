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

EditorContext *EditorWindow::GetEditorContext() const {
  return editor_context_;
}

void EditorWindow::SetEditor(Editor *editor) {
  editor_ = editor;
}

void EditorWindow::SetEditorContext(EditorContext *editor_context) {
  editor_context_ = editor_context;
}

std::string_view EditorWindow::GetName() const {
  return name_;
}

} // namespace Yuggoth