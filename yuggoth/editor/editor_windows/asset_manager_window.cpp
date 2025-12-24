#include "asset_manager_window.h"
#include "yuggoth/core/tools/include/image_wrapper.h"
#include "yuggoth/core/tools/include/core.h"
#include "external/fonts/IconsFontAwesome6.h"
#include "imgui.h"

namespace Yuggoth {

AssetManagerWindow::AssetManagerWindow() {
  ImageWrapper image(GetYuggothRoot() / "assets" / "icons" / "shape.png");
  folder_image_ = Image2D(image.GetWidth(), image.GetHeight(), image.GetData(), SamplerSpecification());
}

void AssetManagerWindow::DrawAssets() {
  if (ImGui::BeginTable("Assets", 5, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody)) {
    for (auto i = 0; i < 10; ++i) {
      ImGui::TableNextColumn();
      ImGui::Image((ImTextureID)(intptr_t)&folder_image_, ImVec2(64, 64));
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