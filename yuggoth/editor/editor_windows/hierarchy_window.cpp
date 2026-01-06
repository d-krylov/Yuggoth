#include "hierarchy_window.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/components/components.h"
#include "yuggoth/editor/core/editor.h"
#include "yuggoth/asset/core/asset_manager.h"
#include "yuggoth/scene/core/scene_manager.h"
#include "external/fonts/IconsFontAwesome6.h"
#include "imgui.h"

namespace Yuggoth {

void HierarchyWindow::AddEntity() {
  auto scene = GetEditorContext()->scene_manager_->GetCurrentScene();
  auto asset_manager = GetEditorContext()->asset_manager_;

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

    if (ImGui::Selectable("Box")) {
      auto hexahedron_entity = scene->CreateEntityWithName("Box");
      hexahedron_entity.AddComponent<Transform>();
      auto &model_component = hexahedron_entity.AddComponent<ModelComponent>();
      model_component.model_ = asset_manager->RegisterModel(PrimitiveKind::HEXAHEDRON);
    }

    if (ImGui::Selectable("Sphere")) {
      scene->CreateEntityWithName("Sphere");
    }

    if (ImGui::Selectable("Octahedron")) {
      auto octahedron_entity = scene->CreateEntityWithName("Octahedron");
      octahedron_entity.AddComponent<Transform>();
      auto &model_component = octahedron_entity.AddComponent<ModelComponent>();
      model_component.model_ = asset_manager->RegisterModel(PrimitiveKind::OCTAHEDRON);
    }

    if (ImGui::Selectable("Torus")) {
      scene->CreateEntityWithName("Torus");
    }

    if (ImGui::Selectable("Cylinder")) {
      scene->CreateEntityWithName("Cylinder");
    }

    ImGui::EndMenu();
  }
}

void HierarchyWindow::DrawNodes() {
  if (GetEditorContext()->scene_manager_->HasValidScenes() == false) return;

  auto scene = GetEditorContext()->scene_manager_->GetCurrentScene();
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

  auto scene_manager = GetEditorContext()->scene_manager_;

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

  ImGui::SameLine();

  std::string_view current_scene_name;

  if (scene_manager->HasValidScenes()) {
    current_scene_name = scene_manager->GetCurrentScene()->GetName();
  }

  if (ImGui::BeginCombo("Scenes", current_scene_name.data())) {
    for (int i = 0; i < scene_manager->GetNumberScenes(); i++) {
      auto scene_name = scene_manager->GetSceneWithIndex(i)->GetName();
      auto is_selected = (current_scene_name == scene_name);
      if (ImGui::Selectable(scene_name.data(), is_selected)) {
        current_scene_name = scene_name;
        scene_manager->SwitchScene(i);
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
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