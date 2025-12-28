#include "viewport_window.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

vector2i ViewportWindow::GetViewportSize() const {
  auto region_size = ImGui::GetContentRegionAvail();
  return vector2i(region_size.x, region_size.y);
}

void ViewportWindow::OnImGui() {
  auto scene_manager = GetEditorContext()->scene_manager_;
  auto current_scene = scene_manager->GetCurrentScene();

  Matrix4f view(1.0f);
  Matrix4f projection(1.0f);
  Matrix4f model(1.0f);

  if (current_scene) {
    auto camera = current_scene->GetCurrentCamera();
    if (camera) {
      view = camera->GetLookAt();
      projection = camera->GetProjection();
    }
  }

  ImGui::Begin("Viewport");
  auto size = ImGui::GetContentRegionAvail();
  auto position = ImGui::GetWindowPos();

  auto renderer = GetEditorContext()->renderer_;

  auto current_viewport_size = GetViewportSize();
  if (current_viewport_size != viewport_size_) {
    renderer->OnViewportResize(current_viewport_size.first, current_viewport_size.second);
    viewport_size_ = current_viewport_size;
  }

  auto &image = renderer->GetImage();

  ImGui::Image((ImTextureID)(intptr_t)&image, ImVec2(viewport_size_.first, viewport_size_.second));

  ImGui::End();
}

} // namespace Yuggoth