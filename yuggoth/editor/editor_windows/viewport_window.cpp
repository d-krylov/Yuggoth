#include "viewport_window.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/scene/core/scene_manager.h"
#include "yuggoth/graphics_device/graphics_device.h"

namespace Yuggoth {

vector2i ViewportWindow::GetViewportSize() const {
  return viewport_size_;
}

void ViewportWindow::OnImGui() {
  auto scene_manager = GetEditorContext()->scene_manager_;
  auto current_scene = scene_manager->GetCurrentScene();

  ImGui::Begin("Viewport");
  auto size = ImGui::GetContentRegionAvail();
  auto position = ImGui::GetWindowPos();

  auto &image = GraphicsDevice::Get()->GetCurrentFrame().target_image_;

  auto region_size = ImGui::GetContentRegionAvail();

  viewport_size_ = vector2i(region_size.x, region_size.y);

  if (image.GetImageLayout() != Walle::ImageLayout::E_UNDEFINED) {
    ImGui::Image((ImTextureID)(intptr_t)&image, ImVec2(viewport_size_.first, viewport_size_.second));
  }

  ImGui::End();
}

} // namespace Yuggoth