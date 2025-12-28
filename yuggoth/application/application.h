#ifndef YUGGOTH_APPLICATION_H
#define YUGGOTH_APPLICATION_H

#include "yuggoth.h"
#include "window_manager.h"

namespace Yuggoth {

class Application {
public:
  Application();

  void Run();

  void OnImGui();
  void OnEvent(Event &event);

  bool OnApplicationExit(const WindowCloseEvent &event);

  void OnStart();

protected:
  void CreateSynchronizationObjects();

private:
  WindowManager window_manager_;
  BufferManager buffer_manager_;
  ShaderLibrary shader_library_;
  PipelineLibrary pipeline_library_;
  ImGuiHost imgui_host_;
  ImGuiRenderer imgui_renderer_;
  AssetManager asset_manager_;
  SceneManager scene_manager_;
  Renderer renderer_;
  Editor editor_;
  bool running_ = false;
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_H