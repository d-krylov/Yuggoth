#ifndef YUGGOTH_APPLICATION_H
#define YUGGOTH_APPLICATION_H

#include "yuggoth.h"

namespace Yuggoth {

class Application {
public:
  Application();

  void Run();

  void OnImGui();
  void OnEvent(Event &event);

  static Application *Get();

  SceneManager *GetSceneManager();

  void OnStart();

protected:
  void CreateSynchronizationObjects();

private:
  Window main_window_;
  GraphicsContext graphics_context_;
  GraphicsAllocator graphics_allocator_;
  Swapchain swapchain_;
  CommandPool command_pool_;
  ImGuiLayer imgui_layer_;
  ImGuiRenderer imgui_renderer_;
  SceneManager scene_manager_;
  Editor editor_;
  std::vector<CommandBuffer> command_buffers_;
  std::vector<Semaphore> render_finished_semaphores_;
  std::vector<Semaphore> image_available_semaphores_;
  std::vector<Fence> frame_fences_;
  uint32_t frame_index_ = 0;

  static Application *application_instance_;
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_H