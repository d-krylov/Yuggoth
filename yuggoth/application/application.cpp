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
  : main_window_(800, 600, "Yuggoth Engine"),   //
    graphics_context_(),                        //
    swapchain_(main_window_.GetNativeWindow()), //
    command_pool_(GraphicsContext::Get()->GetGraphicsQueueIndex()) {
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
      }
      continue;
    } else {
      VK_CHECK(status);
    }

    fence.Reset();
    command_buffer.Reset();
    command_buffer.Begin();

    std::array<VkRenderingAttachmentInfo, 1> rendering_ai = {};
    {
      rendering_ai[0].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
      rendering_ai[0].imageView = swapchain_.GetCurrentImageView();
      rendering_ai[0].imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      rendering_ai[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      rendering_ai[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      rendering_ai[0].clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};
    }

    command_buffer.CommandBeginRendering(swapchain_.GetExtent(), rendering_ai);

    command_buffer.CommandSetViewport(0.0f, 0.0f, swapchain_.GetExtent().width, swapchain_.GetExtent().height);
    command_buffer.CommandSetScissor(0, 0, swapchain_.GetExtent().width, swapchain_.GetExtent().height);

    command_buffer.CommandEndRendering();

    auto present_barrier = GetImageMemoryBarrier(swapchain_.GetCurrentImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0,
                                                 GetImageSubresourceRange(ImageAspectMaskBits::E_COLOR_BIT));

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