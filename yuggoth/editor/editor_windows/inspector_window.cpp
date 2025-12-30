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
  auto far = camera.GetFar();
  ImGui::DragFloat3("Position", glm::value_ptr(position));
  ImGui::SliderAngle("Yaw", &orientation.x);
  ImGui::SliderAngle("Pitch", &orientation.y);
  ImGui::SliderAngle("Roll", &orientation.z);
  ImGui::DragFloat("Far", &far);
  camera.SetFar(far);
  camera.SetPosition(position);
  camera.SetOrientation(orientation);
}

void DrawTransformWidget(Transform &transform) {
  auto position = transform.GetTranslation();
  auto rotation = transform.GetRotation();
  auto scale = transform.GetScale();
  ImGui::DragFloat3("Translation", glm::value_ptr(position));
  ImGui::DragFloat3("Scale", glm::value_ptr(scale));
  ImGui::SliderAngle("X", &rotation.x);
  ImGui::SliderAngle("Y", &rotation.y);
  ImGui::SliderAngle("Z", &rotation.z);
  transform.SetTranslation(position);
  transform.SetScale(scale);
  transform.SetOrientation(rotation);
}

void DrawLightWidget(Light &light) {
  ImGui::DragFloat3("Position", glm::value_ptr(light.position_));
  ImGui::DragFloat3("Color", glm::value_ptr(light.color_), 0.001f, 0.0f, 1.0f);
  ImGui::DragFloat3("Attenuation", glm::value_ptr(light.attenuation), 0.001f, 0.0f, 1.0f);
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