#include "inspector_window.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/components/components.h"
#include "yuggoth/editor/core/editor.h"
#include "imgui.h"

namespace Yuggoth {

void DrawCameramWidget(Camera &camera) {
  auto position = camera.GetPosition();
  auto orientation = camera.GetEulerAngles();
  ImGui::DragFloat3("Position", glm::value_ptr(position));
  ImGui::SliderAngle("Yaw", &orientation.x);
  ImGui::SliderAngle("Pitch", &orientation.y);
  ImGui::SliderAngle("Roll", &orientation.z);

  camera.SetPosition(position);
  camera.SetOrientation(orientation);
}

void DrawTransformWidget(Transform &transform) {
  auto position = transform.GetTranslation();
  auto scale = transform.GetScale();
  ImGui::DragFloat3("Translation", glm::value_ptr(position));
  ImGui::DragFloat3("Scale", glm::value_ptr(scale));
  transform.SetTranslation(position);
  transform.SetScale(scale);
}

void DrawLightWidget(Light &light) {
  ImGui::DragFloat3("Position", glm::value_ptr(light.position_));
  ImGui::DragFloat3("Color", glm::value_ptr(light.color_));
}

void InspectorWindow::OnImGui() {
  ImGui::Begin("Inspector");

  auto selection_manager = GetEditor()->GetSelectionManager();
  auto selected_entities = selection_manager->GetSelectedEntities();

  for (const auto &selected_entity : selected_entities) {
    if (selected_entity.HasComponent<Camera>()) {
      auto &camera = selected_entity.GetComponent<Camera>();
      DrawCameramWidget(camera);
    }
    if (selected_entity.HasComponent<Transform>()) {
      auto &transform = selected_entity.GetComponent<Transform>();
      DrawTransformWidget(transform);
    }

    if (selected_entity.HasComponent<Light>()) {
      auto &light = selected_entity.GetComponent<Light>();
      DrawLightWidget(light);
    }
  }

  ImGui::End();
}

} // namespace Yuggoth