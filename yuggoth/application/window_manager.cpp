#include "window_manager.h"
#include <GLFW/glfw3.h>
#include <format>
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

WindowManager::WindowManager()
  : window_(1920, 1080, "Yuggoth Engine"), //
    surface_(window_.CreateSurface(GraphicsContext::Get()->GetInstance())), swapchain_(surface_) {
  command_pool_ = CommandPool(GraphicsContext::Get()->GetGraphicsQueueIndex());
  Create();
}

void WindowManager::Create() {
  auto object_count = swapchain_.GetNumberOfImages();
  image_available_semaphores_.resize(object_count);
  render_finished_semaphores_.resize(object_count);

  for (auto i = 0; i < object_count; ++i) {
    frame_fences_.emplace_back(Walle::FenceCreateMaskBits::E_SIGNALED_BIT);
    command_buffers_.emplace_back(command_pool_.GetHandle());
  }
}

CommandBuffer *WindowManager::BeginFrame() {
  auto &current_fence = frame_fences_[frame_index_];
  auto &current_command_buffer = command_buffers_[frame_index_];
  auto &current_image_available_semaphore = image_available_semaphores_[frame_index_];

  current_fence.Wait();

  auto status = swapchain_.AcquireNextImage(current_image_available_semaphore.GetHandle());

  if ((status == VK_ERROR_OUT_OF_DATE_KHR) || (status == VK_SUBOPTIMAL_KHR)) {
    if (status == VK_ERROR_OUT_OF_DATE_KHR) {
      swapchain_.Recreate(surface_);
    }
    return nullptr;
  } else {
    VK_CHECK(status);
  }

  current_fence.Reset();
  current_command_buffer.Reset();
  current_command_buffer.Begin(Walle::CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);

  return &current_command_buffer;
}

void WindowManager::EndFrame(CommandBuffer &command_buffer) {
  command_buffer.End();

  auto image_index = swapchain_.GetImageIndex();

  auto &image_available_semaphore = image_available_semaphores_[frame_index_];
  auto &render_finished_semaphore = render_finished_semaphores_[image_index];
  auto &fence = frame_fences_[frame_index_];

  Walle::PipelineStageMask wait_stage = Walle::PipelineStageMaskBits::E_COLOR_ATTACHMENT_OUTPUT_BIT;

  Walle::SubmitInfo submit;
  submit.waitSemaphoreCount = 1;
  submit.pWaitSemaphores = image_available_semaphore.get();
  submit.pWaitDstStageMask = &wait_stage;
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = command_buffer.get();
  submit.signalSemaphoreCount = 1;
  submit.pSignalSemaphores = render_finished_semaphore.get();

  VK_CHECK(vkQueueSubmit(GraphicsContext::Get()->GetGraphicsQueue(), 1, submit, fence.GetHandle()));

  swapchain_.Present(render_finished_semaphore.get());

  frame_index_ = (frame_index_ + 1) % swapchain_.GetNumberOfImages();
}

void WindowManager::SetEventHandler(EventHandler handler) {
  window_.SetEventHandler(handler);
}

const Swapchain *WindowManager::GetSwapchain() const {
  return &swapchain_;
}

const Window *WindowManager::GetWindow() const {
  return &window_;
}

void WindowManager::PollEvents() {
  glfwPollEvents();
}

double WindowManager::GetTime() {
  return glfwGetTime();
}

const Surface *WindowManager::GetSurface() const {
  return &surface_;
}

} // namespace Yuggoth