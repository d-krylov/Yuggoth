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

bool ImGuiLayer::OnKeyEvent(const KeyEvent &event) {
  auto action = event.GetAction();
  if (action != Action::PRESS && action != Action::RELEASE) {
    return true;
  }
  auto &io = ImGui::GetIO();
  auto key = ToImGuiKey(event.GetKey());
  UpdateKeyModifiers(window_);
  io.AddKeyEvent(key, (action == Action::PRESS));
  io.SetKeyEventNativeData(key, (int32_t)event.GetKey(), event.GetScanCode());
  return true;
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

void ImGuiLayer::UpdateMouseData() {
  ImGuiIO &io = ImGui::GetIO();
  auto cursor_position = window_->GetCursorPosition();
  io.AddMousePosEvent((float)cursor_position.x, (float)cursor_position.y);
}

ImGuiLayer::ImGuiLayer(Window *window) : window_(window) {
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  SetStyle();
}

void ImGuiLayer::SetStyle() {
  ImGuiStyle &style = ImGui::GetStyle();

  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ImGuiLayer::OnEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseMoveEvent>(BIND_FUNCTION(ImGuiLayer::OnMouseMoveEvent));
  dispatcher.Dispatch<MouseButtonEvent>(BIND_FUNCTION(ImGuiLayer::OnMouseButtonEvent));
  dispatcher.Dispatch<MouseScrollEvent>(BIND_FUNCTION(ImGuiLayer::OnMouseScrollEvent));
  dispatcher.Dispatch<CharEvent>(BIND_FUNCTION(ImGuiLayer::OnCharEvent));
  dispatcher.Dispatch<KeyEvent>(BIND_FUNCTION(ImGuiLayer::OnKeyEvent));
}

void ImGuiLayer::UpdateTime() {
  auto current_time = glfwGetTime();
  auto &io = ImGui::GetIO();
  if (current_time <= time_) {
    current_time = time_ + 0.00001f;
  }
  io.DeltaTime = time_ > 0.0 ? (current_time - time_) : (1.0f / 60.0f);
  time_ = current_time;
}

void ImGuiLayer::NewFrame() {
  auto &io = ImGui::GetIO();

  auto window_size = window_->GetWindowSize();
  auto framebuffer_size = window_->GetFramebufferSize();

  io.DisplaySize.x = window_size.width;
  io.DisplaySize.y = window_size.height;

  if (window_size.width > 0 && window_size.height > 0) {
    io.DisplayFramebufferScale.x = static_cast<float>(framebuffer_size.width) / static_cast<float>(window_size.width);
    io.DisplayFramebufferScale.y = static_cast<float>(framebuffer_size.height) / static_cast<float>(window_size.height);
  }

  UpdateTime();
  UpdateMouseData();
}

} // namespace Yuggoth
