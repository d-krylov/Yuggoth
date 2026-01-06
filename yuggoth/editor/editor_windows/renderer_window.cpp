#include "renderer_window.h"
#include "yuggoth/renderer/shaders/shader_library.h"
#include "imgui.h"

namespace Yuggoth {

void RendererWindow::OnImGui() {
  ImGui::Begin("Renderer");

  if (ImGui::BeginTabBar("Renderer Components")) {
    if (ImGui::BeginTabItem("Shader Library")) {
      DrawShaderLibrary();
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Performance")) {
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::End();
}

void RendererWindow::DrawShaderInformation(const std::filesystem::path &path) {
  auto editor_context = GetEditorContext();
  auto shader_library = editor_context->shader_library_;
  auto &shader_modules = shader_library->GetShaderModules();
  if (show_shader_information_) {
    ImGui::Begin("Shader Information", &show_shader_information_);
    const auto &shader_module = shader_modules.at(path);
    const auto &bindings = shader_module->GetDescriptorSetBindings();
    for (const auto &[name, binding] : bindings) {
      ImGui::Text("Name: %s, Set: %d, Binding: %d, Count: %d", name.c_str(), binding.set_, binding.binding_, binding.count_);
    }
    ImGui::End();
  }
}

void RendererWindow::DrawShaderLibrary() {
  auto editor_context = GetEditorContext();
  auto shader_library = editor_context->shader_library_;
  auto &shader_modules = shader_library->GetShaderModules();
  for (const auto &[name, module] : shader_modules) {
    if (ImGui::Selectable(name.c_str())) {
      show_shader_information_ = true;
      shader_path = name;
    }
  }

  DrawShaderInformation(shader_path);
}

} // namespace Yuggoth