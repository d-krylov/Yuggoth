#ifndef YUGGOTH_APPLICATION_H
#define YUGGOTH_APPLICATION_H

#include "yuggoth/gui/window/window.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/graphics/graphics_context/graphics_allocator.h"
#include "yuggoth/graphics/presentation/swapchain.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/command/command_pool.h"
#include "yuggoth/graphics/core/synchronization.h"
#include "yuggoth/gui/imgui/imgui_layer.h"
#include "yuggoth/gui/imgui/imgui_renderer.h"
#include "yuggoth/editor/core/editor.h"

namespace Yuggoth {

class Application {
public:
  Application();

  void Run();

  void OnImGui();
  void OnEvent(Event &event);

  static Application *Get();

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