#ifndef YUGGOTH_EDITOR_H
#define YUGGOTH_EDITOR_H

#include "yuggoth/editor/editor_windows/editor_windows.h"
#include "selection_manager.h"

namespace Yuggoth {

class Editor {
public:
  Editor();

  void OnImGui();

  template <typename T> T &GetEditorWindow();

  SelectionManager *GetSelectionManager();

  void SetEditorContext(const EditorContext &editor_context);

  bool InDebugMode() const;

protected:
  void DrawMainMenu();
  void HanldeDialog();

private:
  EditorContext editor_context_;
  ViewportWindow viewport_window_;
  HierarchyWindow hierarchy_window_;
  InspectorWindow inspector_window_;
  ApplicationWindow application_window_;
  AssetManagerWindow asset_manager_window_;
  RendererWindow renderer_window_;
  SelectionManager selection_manager_;
  std::string dialog_name_;
  bool debug_mode_ = false;
};

} // namespace Yuggoth

#include "editor.ipp"

#endif // YUGGOTH_EDITOR_H