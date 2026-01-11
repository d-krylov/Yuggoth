#include "application.h"
#include <algorithm>
#include <cassert>
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Application::Application()
  : window_manager_(),                                                                 //
    buffer_manager_(), imgui_host_(window_manager_.GetWindow()),                       //
    shader_library_(),                                                                 //
    pipeline_library_(&shader_library_),                                               //
    imgui_renderer_(window_manager_.GetSwapchain()->GetFormat()),                      //
    material_manager_(&asset_manager_),                                                //
    asset_manager_(AssetManagerCreateContext(&buffer_manager_, &material_manager_)),   //
    scene_manager_(&asset_manager_),                                                   //
    renderer_(RendererContext(&pipeline_library_, &buffer_manager_, &asset_manager_)), //
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
  editor_context.renderer_ = &renderer_;
  editor_context.asset_manager_ = &asset_manager_;
  editor_context.buffer_manager_ = &buffer_manager_;
  editor_context.shader_library_ = &shader_library_;
  editor_context.renderer_resources_ = &renderer_resources_;
  editor_.SetEditorContext(editor_context);
}

void Application::Run() {
  while (running_) {

    WindowManager::PollEvents();

    auto new_viewport_size = editor_.GetEditorWindow<ViewportWindow>().GetViewportSize();
    if (new_viewport_size.first != 0 && new_viewport_size.second != 0) {
      renderer_resources_.GetCurrentFrame().OnResize(new_viewport_size.first, new_viewport_size.second);
    }

    auto command_buffer = window_manager_.BeginFrame();
    auto swapchain = window_manager_.GetSwapchain();

    if (command_buffer == nullptr) continue;

    asset_manager_.Update();

    auto subresource = GetImageSubresourceRange();
    command_buffer->TransitionImageLayout(swapchain->GetCurrentImage(), ImageLayout::E_UNDEFINED, ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT, AccessMaskBits2::E_NONE,
                                          AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT, subresource);

    imgui_host_.NewFrame();
    imgui_renderer_.Begin(*command_buffer);

    OnImGui();

    renderer_resources_.BeginFrame();

    auto &current_frame = renderer_resources_.GetCurrentFrame();

    auto extent = current_frame.target_image_.GetImageCreateInformation().extent_;

    renderer_.Begin(scene_manager_.GetCurrentScene(), current_frame.command_buffer_, current_frame.target_image_, current_frame.depth_image_);
    renderer_.Draw(scene_manager_.GetCurrentScene(), current_frame.command_buffer_, Extent2D(extent.width, extent.height));

    // renderer_.DrawRayTrace(scene_manager_.GetCurrentScene());
    renderer_.End();
    renderer_resources_.EndFrame();

    imgui_renderer_.End(*command_buffer, *swapchain);

    command_buffer->TransitionImageLayout(swapchain->GetCurrentImage(), ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL, ImageLayout::E_PRESENT_SRC_KHR,
                                          PipelineStageMaskBits2::E_COLOR_ATTACHMENT_OUTPUT_BIT, PipelineStageMaskBits2::E_NONE,
                                          AccessMaskBits2::E_COLOR_ATTACHMENT_WRITE_BIT, AccessMaskBits2::E_NONE, subresource);

    window_manager_.EndFrame(*command_buffer);
  }

  vkDeviceWaitIdle(GraphicsContext::Get()->GetDevice());
}

} // namespace Yuggoth