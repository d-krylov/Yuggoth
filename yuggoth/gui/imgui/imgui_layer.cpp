#include "imgui_layer.h"
#include "imgui.h"
#include "yuggoth/gui/window/window.h"
#include <GLFW/glfw3.h>

namespace Yuggoth {

void UpdateKeyModifiers(const Window *window) {
  auto &io = ImGui::GetIO();
  io.AddKeyEvent(ImGuiMod_Ctrl, (window->GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Shift, (window->GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Alt, (window->GetKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Super, (window->GetKey(GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));
}

bool ImGuiLayer::OnMouseButtonEvent(const MouseButtonEvent &event) {
  auto &io = ImGui::GetIO();
  UpdateKeyModifiers(window_);
  io.AddMouseButtonEvent(int32_t(event.GetMouseButton()), event.GetAction() == Action::PRESS);
  return true;
}

bool ImGuiLayer::OnMouseScrollEvent(const MouseScrollEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddMouseWheelEvent(event.GetX(), event.GetY());
  return true;
}

bool ImGuiLayer::OnMouseMoveEvent(const MouseMoveEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddMousePosEvent(event.GetX(), event.GetY());
  return true;
}

bool ImGuiLayer::OnCharEvent(const CharEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddInputCharacter(event.GetCharacter());
  return true;
}

ImGuiLayer::ImGuiLayer(Window *window) : window_(window) {
  ImGui::CreateContext();
}

} // namespace Yuggoth
