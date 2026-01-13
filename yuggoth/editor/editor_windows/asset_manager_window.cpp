#include "asset_manager_window.h"
#include "yuggoth/asset/core/asset_manager.h"
#include "yuggoth/core/tools/core.h"
#include "external/fonts/IconsFontAwesome6.h"
#include "imgui.h"

namespace Yuggoth {

AssetManagerWindow::AssetManagerWindow() {
}

void AssetManagerWindow::DrawAssets() {
  auto asset_manager = GetEditorContext()->asset_manager_;
  const auto &assets = asset_manager->GetAssets();

  if (ImGui::BeginTable("Assets", 5, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody)) {
    for (const auto &[uuid, asset] : assets) {
      ImGui::TableNextColumn();
      if (asset->GetAssetKind() == AssetKind::IMAGE) {
        auto image = static_pointer_cast<Image>(asset);
        ImGui::Image((ImTextureID)(intptr_t)image.get(), ImVec2(100, 100));
        ImGui::Text("UUID: %lx", uuid.GetValue());
      }
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