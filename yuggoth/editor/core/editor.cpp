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
}

void Editor::SetEditorContext(const EditorContext &editor_context) {
  editor_context_ = editor_context;

  viewport_window_.SetEditor(this);
  hierarchy_window_.SetEditor(this);
  inspector_window_.SetEditor(this);
  application_window_.SetEditor(this);
  asset_manager_window_.SetEditor(this);

  viewport_window_.SetEditorContext(&editor_context_);
  hierarchy_window_.SetEditorContext(&editor_context_);
  inspector_window_.SetEditorContext(&editor_context_);
  application_window_.SetEditorContext(&editor_context_);
  asset_manager_window_.SetEditorContext(&editor_context_);
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
  auto scene_manager = editor_context_.scene_manager_;
  auto asset_manager = editor_context_.asset_manager_;

  if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      if (scene_manager->HasValidScenes() == false) {
        scene_manager->EnqueueScene();
      }
      auto current_scene = scene_manager->GetCurrentScene();
      std::filesystem::path path = ImGuiFileDialog::Instance()->GetFilePathName();
      auto model_entity = current_scene->CreateEntityWithName(path.filename().c_str());

      auto &model_component = model_entity.AddComponent<ModelComponent>();
      model_entity.AddComponent<Transform>();

      model_component.model_ = asset_manager->RegisterModel(path);
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
