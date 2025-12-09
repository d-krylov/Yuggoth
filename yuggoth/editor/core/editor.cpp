#include "editor.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "yuggoth/application/application.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/components/components.h"
#include "external/fonts/IconsFontAwesome6.h"
#include <print>

namespace Yuggoth {

Editor::Editor() {
  hierarchy_window_.SetEditor(this);
  inspector_window_.SetEditor(this);
}

SelectionManager *Editor::GetSelectionManager() {
  return &selection_manager_;
}

void Editor::OnImGui() {

  DrawMainMenu();

  viewport_window_.OnImGui();
  hierarchy_window_.OnImGui();
  inspector_window_.OnImGui();
  application_window_.OnImGui();
  asset_manager_window_.OnImGui();

  ImportFile();
}

void Editor::ImportFile() {
  auto scene_manager = Application::Get()->GetSceneManager();
  if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      if (scene_manager->HasValidScenes() == false) {
        scene_manager->EnqueueScene();
      }
      auto current_scene = scene_manager->GetCurrentScene();
      std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
      auto model_entity = current_scene->CreateEntityWithName("Model");
      model_entity.AddComponent<ModelComponent>(path);
      model_entity.AddComponent<Transform>();
    }

    ImGuiFileDialog::Instance()->Close();
  }
}

void Editor::DrawMainMenu() {
  if (ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {

      if (ImGui::MenuItem(ICON_FA_FILE " New", "Ctrl+N")) {
      }

      if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "Ctrl+O")) {
      }

      ImGui::Separator();

      if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "Ctrl+S")) {
      }

      ImGui::Separator();

      if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Import")) {
        IGFD::FileDialogConfig config;
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Choose File", ".gltf,.glb", config);
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
}

} // namespace Yuggoth
