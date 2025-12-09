#include "viewport_window.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "yuggoth/application/application.h"
#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

void ViewportWindow::OnImGui() {
  auto scene_manager = Application::Get()->GetSceneManager();
  auto current_scene = scene_manager->GetCurrentScene();

  Matrix4f view(1.0f);
  Matrix4f proj(1.0f);

  if (current_scene) {
    auto camera = current_scene->GetCurrentCamera();
    if (camera) {
      view = camera->GetLookAt();
      proj = camera->GetProjection();
    }
  }

  auto m = Matrix4f(1.0f);
  ImGui::Begin("Viewport");
  auto size = ImGui::GetContentRegionAvail();
  auto position = ImGui::GetWindowPos();

  auto renderer = Application::Get()->GetSceneRenderer();

  auto &image = renderer->GetImage();
  auto &extent = image.GetExtent();

  ImGui::Image((ImTextureID)(intptr_t)&image, ImVec2(extent.width, extent.height));

  ImGuizmo::SetDrawlist();
  ImGuizmo::SetRect(position.x, position.y, size.x, size.y);
  ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(proj), glm::value_ptr(m), 200.0f);

  ImGui::End();
}

} // namespace Yuggoth