#include "hierarchy_window.h"
#include "yuggoth/application/application.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/components/components.h"
#include "imgui.h"

namespace Yuggoth {

void HierarchyWindow::AddEntity() {
  auto scene = Application::Get()->GetSceneManager()->GetCurrentScene();

  if (ImGui::MenuItem("Camera")) {
    scene->CreateEntityWithName("Camera");
  }
  if (ImGui::MenuItem("Light")) {
    scene->CreateEntityWithName("Light");
  }
}

void HierarchyWindow::DrawNodes() {
  if (Application::Get()->GetSceneManager()->HasValidScenes() == false) return;

  auto scene = Application::Get()->GetSceneManager()->GetCurrentScene();
  auto &registry = scene->GetRegistry();

  for (auto [raw_entity] : registry.storage<entt::entity>().each()) {
    Entity entity(raw_entity, scene);
    auto &name = entity.GetComponent<NameComponent>();
    ImGui::Text("%s", name.name_.c_str());
  }
}

void HierarchyWindow::OnImGui() {
  ImGui::Begin("Scene");

  auto scene_manager = Application::Get()->GetSceneManager();

  ImGui::BeginDisabled(!scene_manager->HasValidScenes());
  if (ImGui::Button("Add")) {
    ImGui::OpenPopup("AddEntity");
    ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
  }
  ImGui::EndDisabled();

  if (ImGui::BeginPopup("AddEntity")) {
    AddEntity();
    ImGui::EndPopup();
  }

  ImGui::SameLine();
  if (ImGui::Button("New Scene")) {
    scene_manager->EnqueueScene();
  }

  ImGui::Separator();
  DrawNodes();
  ImGui::End();
}

} // namespace Yuggoth