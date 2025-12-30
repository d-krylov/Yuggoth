#include "editor.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "editor_tools.h"
#include "yuggoth/scene/core/entity.h"
#include "yuggoth/scene/core/scene_manager.h"
#include "yuggoth/asset/include/asset_manager.h"
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
  renderer_window_.SetEditorContext(&editor_context_);
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
  renderer_window_.OnImGui();

  HanldeDialog();
}

void Editor::HanldeDialog() {
  auto path = DrawFileDialog(dialog_name_);

  if (path.has_value()) {

    auto scene_manager = editor_context_.scene_manager_;
    auto asset_manager = editor_context_.asset_manager_;

    if (scene_manager->HasValidScenes() == false) {
      scene_manager->EnqueueScene();
    }

    auto current_scene = scene_manager->GetCurrentScene();
    auto model_entity = current_scene->CreateEntityWithName(path.value().filename().c_str());

    model_entity.AddComponent<Transform>();
    auto &model_component = model_entity.AddComponent<ModelComponent>();

    if (dialog_name_ == "ImportModel") {
      model_component.model_ = asset_manager->RegisterModel(path.value());
    }
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

      if (ImGui::BeginMenu(ICON_FA_FILE_IMPORT "Import")) {
        if (ImGui::MenuItem("Model")) {
          IGFD::FileDialogConfig config;
          dialog_name_ = "ImportModel";
          ImGuiFileDialog::Instance()->OpenDialog(dialog_name_, "Import Model", ".gltf,.glb", config);
        }

        ImGui::EndMenu();
      }

      ImGui::Separator();

      if (ImGui::MenuItem(ICON_FA_POWER_OFF "Exit")) {
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Editor")) {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Save GUI Settings")) {
        ImGui::SaveIniSettingsToDisk("setting.ini");
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
      ImGui::EndMenu();
    }

    ImGui::Checkbox("Debug Mode", &debug_mode_);

    ImGui::EndMainMenuBar();
  }
}

bool Editor::InDebugMode() const {
  return debug_mode_;
}

} // namespace Yuggoth
