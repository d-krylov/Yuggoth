#include "inspector_window.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include "imgui.h"

namespace Yuggoth {

Vector3f position(0.0f);
Vector3f rotation(0.0f);

void DrawTransformWidget() {
  ImGui::DragFloat3("Position", glm::value_ptr(position));
  ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 360.0f);
}

void InspectorWindow::OnImGui() {
  ImGui::Begin("Inspector");
  DrawTransformWidget();
  ImGui::End();
}

} // namespace Yuggoth