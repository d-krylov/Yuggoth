#include "editor_tools.h"
#include "ImGuiFileDialog.h"

namespace Yuggoth {

std::optional<std::filesystem::path> DrawFileDialog(std::string_view name) {
  std::optional<std::filesystem::path> path;
  if (ImGuiFileDialog::Instance()->Display(name.data())) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      path = ImGuiFileDialog::Instance()->GetFilePathName();
    }
    ImGuiFileDialog::Instance()->Close();
  }
  return path;
}

} // namespace Yuggoth