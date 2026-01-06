#include "imgui_host.h"
#include "imgui.h"
#include "yuggoth/gui/window/window.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/core/tools/include/filesystem.h"
#include "external/fonts/IconsFontAwesome6.h"
#include <GLFW/glfw3.h>

namespace Yuggoth {

void UpdateKeyModifiers(const Window *window) {
  auto &io = ImGui::GetIO();
  io.AddKeyEvent(ImGuiMod_Ctrl, (window->GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Shift, (window->GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Alt, (window->GetKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
  io.AddKeyEvent(ImGuiMod_Super, (window->GetKey(GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (window->GetKey(GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));
}

bool ImGuiHost::OnKeyEvent(const KeyEvent &event) {
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

bool ImGuiHost::OnMouseButtonEvent(const MouseButtonEvent &event) {
  auto &io = ImGui::GetIO();
  UpdateKeyModifiers(window_);
  io.AddMouseButtonEvent(int32_t(event.GetMouseButton()), event.GetAction() == Action::PRESS);
  return true;
}

bool ImGuiHost::OnMouseScrollEvent(const MouseScrollEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddMouseWheelEvent(event.GetX(), event.GetY());
  return true;
}

bool ImGuiHost::OnMouseMoveEvent(const MouseMoveEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddMousePosEvent(event.GetX(), event.GetY());
  return true;
}

bool ImGuiHost::OnCharEvent(const CharEvent &event) {
  auto &io = ImGui::GetIO();
  io.AddInputCharacter(event.GetCharacter());
  return true;
}

void ImGuiHost::UpdateMouseData() {
  ImGuiIO &io = ImGui::GetIO();
  auto cursor_position = window_->GetCursorPosition();
  io.AddMousePosEvent((float)cursor_position.first, (float)cursor_position.second);
}

ImGuiHost::ImGuiHost(const Window *window) : window_(window) {
  ImGui::CreateContext();
  auto settings = FileSystem::GetYuggothRootDirectory() / "assets" / "settings" / "window_settings.ini";
  ImGui::LoadIniSettingsFromDisk(settings.c_str());
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  auto base_font_size = 16.0f;
  auto base_font = FileSystem::GetYuggothRootDirectory() / "assets" / "fonts" / "segoe" / "Segoe_UI.ttf";
  io.Fonts->AddFontFromFileTTF(base_font.c_str(), base_font_size);

  auto fonts_fas = FileSystem::GetYuggothRootDirectory() / "assets" / "fonts" / "fa" / FONT_ICON_FILE_NAME_FAS;

  auto icon_font_size = base_font_size;

  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = icon_font_size;

  const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};

  io.Fonts->AddFontFromFileTTF(fonts_fas.c_str(), icon_font_size, &icons_config, icons_ranges);
  io.Fonts->Build();

  SetStyle();
}

void ImGuiHost::SetStyle() {
  ImGuiStyle &style = ImGui::GetStyle();

  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  style.FrameRounding = 3.0f;
  style.PopupRounding = 6.0f;
  style.WindowRounding = 8.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 6.0f;
}

void ImGuiHost::OnEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseMoveEvent>(BIND_FUNCTION(ImGuiHost::OnMouseMoveEvent));
  dispatcher.Dispatch<MouseButtonEvent>(BIND_FUNCTION(ImGuiHost::OnMouseButtonEvent));
  dispatcher.Dispatch<MouseScrollEvent>(BIND_FUNCTION(ImGuiHost::OnMouseScrollEvent));
  dispatcher.Dispatch<CharEvent>(BIND_FUNCTION(ImGuiHost::OnCharEvent));
  dispatcher.Dispatch<KeyEvent>(BIND_FUNCTION(ImGuiHost::OnKeyEvent));
}

void ImGuiHost::UpdateTime() {
  auto current_time = glfwGetTime();
  auto &io = ImGui::GetIO();
  if (current_time <= time_) {
    current_time = time_ + 0.00001f;
  }
  io.DeltaTime = time_ > 0.0 ? (current_time - time_) : (1.0f / 60.0f);
  time_ = current_time;
}

void ImGuiHost::NewFrame() {
  auto &io = ImGui::GetIO();

  auto window_size = window_->GetWindowSize();
  auto framebuffer_size = window_->GetFramebufferSize();

  io.DisplaySize.x = window_size.first;
  io.DisplaySize.y = window_size.second;

  if (window_size.first > 0 && window_size.second > 0) {
    io.DisplayFramebufferScale.x = static_cast<float>(framebuffer_size.first) / static_cast<float>(window_size.first);
    io.DisplayFramebufferScale.y = static_cast<float>(framebuffer_size.second) / static_cast<float>(window_size.second);
  }

  UpdateTime();
  UpdateMouseData();
}

} // namespace Yuggoth
