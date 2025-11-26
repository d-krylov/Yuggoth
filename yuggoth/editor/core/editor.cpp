#include "editor.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include <print>

namespace Yuggoth {

Editor::Editor() {
}

Vector3f v(0.0f);

void Editor::OnImGui() {

  DrawMainMenu();

  viewport_window_.OnImGui();
  hierarchy_window_.OnImGui();
  inspector_window_.OnImGui();
  application_window_.OnImGui();
}

void Editor::DrawMainMenu() {
  if (ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {

      if (ImGui::MenuItem("New", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", nullptr, config);
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) {
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string fileNameOnly = ImGuiFileDialog::Instance()->GetCurrentFileName();

      std::println("Selected file: {}", filePathName.c_str());
    }

    ImGuiFileDialog::Instance()->Close();
  }
}

} // namespace Yuggoth
