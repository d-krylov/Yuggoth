#include "asset_manager_window.h"
#include "yuggoth/core/tools/include/image_wrapper.h"
#include "yuggoth/core/tools/include/core.h"
#include "external/fonts/IconsFontAwesome6.h"
#include "imgui.h"

namespace Yuggoth {

AssetManagerWindow::AssetManagerWindow() {
}

void AssetManagerWindow::DrawAssets() {
  if (ImGui::BeginTable("Assets", 5, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody)) {
    for (auto i = 0; i < 10; ++i) {
      ImGui::TableNextColumn();
      ImGui::Text("Mesh");
    }
    ImGui::EndTable();
  }
}

void AssetManagerWindow::OnImGui() {
  auto asset_manager = GetEditorContext()->asset_manager_;
  ImGui::Begin("Asset Manager");
  DrawAssets();
  ImGui::End();
}

} // namespace Yuggoth