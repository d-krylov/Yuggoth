#ifndef YUGGOTH_RENDERER_WINDOW_H
#define YUGGOTH_RENDERER_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"
#include <filesystem>

namespace Yuggoth {

class RendererWindow : public EditorWindow {
public:
  void OnImGui();

protected:
  void DrawShaderLibrary();
  void DrawShaderInformation(const std::filesystem::path &path);

private:
  bool show_shader_information_ = false;
  std::filesystem::path shader_path;
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_WINDOW_H