#include "asset_manager_window.h"
#include "imgui.h"

namespace Yuggoth {

void AssetManagerWindow::OnImGui() {
  auto editor_context = GetEditorContext();
  auto asset_manager = editor_context->asset_manager_;
  ImGui::Begin("Asset Manager");

  ImGui::End();
}

} // namespace Yuggoth