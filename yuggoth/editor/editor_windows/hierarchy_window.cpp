#include "hierarchy_window.h"
#include "yuggoth/application/application.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/components/components.h"
#include "imgui.h"

namespace Yuggoth {

void HierarchyWindow::AddEntity() {
  auto scene = Application::Get()->GetSceneManager()->GetCurrentScene();

  if (ImGui::Selectable("Camera")) {
    auto camera_entity = scene->CreateEntityWithName("Camera");
    auto &camera = camera_entity.AddComponent<Camera>();
    scene->SetCurrentCamera(&camera);
  }

  if (ImGui::Selectable("Light")) {
    auto light_entity = scene->CreateEntityWithName("Light");
    light_entity.AddComponent<Light>();
  }

  if (ImGui::Selectable("Rigid Body")) {
    scene->CreateEntityWithName("Rigid Body");
  }

  if (ImGui::BeginMenu("Primitive")) {

    ImGui::EndMenu();
  }
}

void HierarchyWindow::DrawNodes() {
  if (Application::Get()->GetSceneManager()->HasValidScenes() == false) return;

  auto scene = Application::Get()->GetSceneManager()->GetCurrentScene();
  auto &registry = scene->GetRegistry();

  for (auto [raw_entity] : registry.storage<entt::entity>().each()) {

    Entity entity(raw_entity, scene);
    auto &name = entity.GetComponent<NameComponent>();
    auto &uuid = entity.GetComponent<UUIDComponent>();

    auto unique_name = std::format("{}##{}", name.name_, uuid.uuid_.GetValue());

    auto selection_manager = GetEditor()->GetSelectionManager();
    auto is_selected = selection_manager->IsEntitySelected(entity);

    if (ImGui::Selectable(unique_name.c_str(), &is_selected)) {
      selection_manager->Clear();
      selection_manager->SetSelected(entity);
    }
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

  if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    auto current_scene = scene_manager->GetCurrentScene();
    auto selection_manager = GetEditor()->GetSelectionManager();
    for (const auto &entity : selection_manager->GetSelectedEntities()) {
      current_scene->DestroyEntity(entity);
    }
  }

  ImGui::Separator();
  DrawNodes();
  ImGui::End();
}

} // namespace Yuggoth