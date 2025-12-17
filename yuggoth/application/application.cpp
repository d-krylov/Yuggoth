#include "application.h"
#include <algorithm>
#include <cassert>
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Application::Application()
  : window_manager_(),                                            //
    buffer_manager_(), imgui_host_(window_manager_.GetWindow()),  //
    imgui_renderer_(window_manager_.GetSwapchain()->GetFormat()), //
    asset_manager_(&buffer_manager_),                             //
    scene_manager_(&asset_manager_),                              //
    scene_renderer_(),                                            //
    editor_() {

  OnStart();
  window_manager_.SetEventHandler(BIND_FUNCTION(Application::OnEvent));
}

void Application::OnImGui() {
  editor_.OnImGui();
}

void Application::OnEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_FUNCTION(Application::OnApplicationExit));
  imgui_host_.OnEvent(event);
}

bool Application::OnApplicationExit(const WindowCloseEvent &event) {
  running_ = false;
  return true;
}

void Application::OnStart() {
  running_ = true;

  EditorContext editor_context;
  editor_context.scene_manager_ = &scene_manager_;
  editor_context.scene_renderer_ = &scene_renderer_;
  editor_context.asset_manager_ = &asset_manager_;
  editor_.SetEditorContext(editor_context);
}

void Application::Run() {
  while (running_) {

    WindowManager::PollEvents();

    auto command_buffer = window_manager_.BeginFrame();
    auto swapchain = window_manager_.GetSwapchain();

    if (command_buffer == nullptr) continue;

    auto subresource = GetImageSubresourceRange();
    command_buffer->TransitionImageLayout(swapchain->GetCurrentImage(), ImageLayout::E_UNDEFINED, ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT, AccessMaskBits2::E_NONE,
                                          AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT, subresource);

    imgui_host_.NewFrame();
    imgui_renderer_.Begin(*command_buffer);

    OnImGui();

    scene_renderer_.Draw(scene_manager_.GetCurrentScene());

    imgui_renderer_.End(*command_buffer, *swapchain);

    command_buffer->TransitionImageLayout(swapchain->GetCurrentImage(), ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, ImageLayout::E_PRESENT_SRC_KHR,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT, PipelineStageMaskBits2::E_NONE,
                                          AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT, AccessMaskBits2::E_NONE, subresource);

    window_manager_.EndFrame(*command_buffer);
  }

  vkDeviceWaitIdle(GraphicsContext::Get()->GetDevice());
}

} // namespace Yuggoth