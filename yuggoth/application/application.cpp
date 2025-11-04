#include "application.h"
#include <algorithm>
#include <cassert>
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Application *Application::application_instance_ = nullptr;

Application *Application::Get() {
  assert(application_instance_ != nullptr);
  return application_instance_;
}

void Application::CreateSynchronizationObjects() {
  auto object_count = swapchain_.GetNumberOfImages();
  image_available_semaphores_.resize(object_count);
  render_finished_semaphores_.resize(object_count);
  frame_fences_.reserve(object_count);
  for (auto i = 0; i < object_count; ++i) {
    frame_fences_.emplace_back(FenceCreateMaskBits::E_SIGNALED_BIT);
  }
  command_buffers_.reserve(object_count);
  for (auto i = 0; i < object_count; ++i) {
    command_buffers_.emplace_back(command_pool_.GetHandle());
  }
}

Application::Application()
  : main_window_(800, 600, "Yuggoth Engine"),                 //
    graphics_context_(),                                      //
    graphics_allocator_(),                                    //
    swapchain_(main_window_.GetNativeWindow()),               //
    command_pool_(graphics_context_.GetGraphicsQueueIndex()), //
    imgui_layer_(&main_window_),                              //
    imgui_renderer_(swapchain_.GetFormat()) {
  CreateSynchronizationObjects();
}

void Application::Run() {

  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  while (main_window_.ShouldClose() == false) {
    main_window_.PollEvents();

    auto &fence = frame_fences_[frame_index_];
    auto &command_buffer = command_buffers_[frame_index_];
    auto &image_available_semaphore = image_available_semaphores_[frame_index_];

    fence.Wait();

    auto status = swapchain_.AcquireNextImage(image_available_semaphore.GetHandle());

    if ((status == VK_ERROR_OUT_OF_DATE_KHR) || (status == VK_SUBOPTIMAL_KHR)) {
      if (status == VK_ERROR_OUT_OF_DATE_KHR) {
        swapchain_.Recreate();
      }
      continue;
    } else {
      VK_CHECK(status);
    }

    fence.Reset();
    command_buffer.Reset();
    command_buffer.Begin();

    imgui_layer_.NewFrame();
    imgui_renderer_.Begin(command_buffer, swapchain_);

    ImGui::Begin("Hello");
    ImGui::End();

    imgui_renderer_.End(command_buffer);

    auto present_barrier = GetImageMemoryBarrier(
      swapchain_.GetCurrentImage(), ImageLayout::E_UNDEFINED, ImageLayout::E_PRESENT_SRC_KHR, PipelineStageMaskBits2::E_TOP_OF_PIPE_BIT,
      PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT, {}, {}, GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT));

    command_buffer.CommandPipelineBarrier(DependencyMaskBits::E_BY_REGION_BIT, {}, std::array{present_barrier});

    command_buffer.End();

    auto &render_finished_semaphore = render_finished_semaphores_[swapchain_.GetImageIndex()];

    VkSubmitInfo submit_info{};
    {
      submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submit_info.waitSemaphoreCount = 1;
      submit_info.pWaitSemaphores = image_available_semaphore.GetPointer();
      submit_info.pWaitDstStageMask = wait_stages;
      submit_info.commandBufferCount = 1;
      submit_info.pCommandBuffers = command_buffer.GetPointer();
      submit_info.signalSemaphoreCount = 1;
      submit_info.pSignalSemaphores = render_finished_semaphore.GetPointer();
    }

    VK_CHECK(vkQueueSubmit(GraphicsContext::Get()->GetGraphicsQueue(), 1, &submit_info, fence.GetHandle()));

    swapchain_.Present(render_finished_semaphore.GetPointer());

    frame_index_ = (frame_index_ + 1) % (swapchain_.GetNumberOfImages() - 1);
  }
}

} // namespace Yuggoth