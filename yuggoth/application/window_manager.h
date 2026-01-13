#ifndef YUGGOTH_WINDOW_MANAGER_H
#define YUGGOTH_WINDOW_MANAGER_H

#include "yuggoth/gui/window/window.h"
#include "yuggoth/graphics/presentation/swapchain.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/command/command_pool.h"
#include "yuggoth/graphics/synchronization/fence.h"
#include "yuggoth/graphics/synchronization/semaphore.h"

namespace Yuggoth {

class WindowManager {
public:
  WindowManager();

  CommandBuffer *BeginFrame();

  void EndFrame(CommandBuffer &command_buffer);

  const Swapchain *GetSwapchain() const;

  const Surface *GetSurface() const;

  const Window *GetWindow() const;

  void SetEventHandler(EventHandler handler);

  static void PollEvents();

  static double GetTime();

protected:
  void Create();

private:
  Window window_;
  Surface surface_;
  Swapchain swapchain_;
  CommandPool command_pool_;
  std::vector<CommandBuffer> command_buffers_;
  std::vector<Semaphore> render_finished_semaphores_;
  std::vector<Semaphore> image_available_semaphores_;
  std::vector<Fence> frame_fences_;
  uint64_t frame_index_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_WINDOW_MANAGER_H