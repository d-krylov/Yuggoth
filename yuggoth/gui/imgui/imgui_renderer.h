#ifndef YUGGOTH_IMGUI_RENDERER_H
#define YUGGOTH_IMGUI_RENDERER_H

#include "yuggoth/graphics/pipeline/graphics_pipeline.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/image/image.h"
#include <memory>

namespace Yuggoth {

class CommandBuffer;
class Swapchain;

class ImGuiRenderer {
public:
  ImGuiRenderer(Walle::Format color_format);

  void RenderDrawData(CommandBuffer &command_buffer);

  void Begin(CommandBuffer &command_buffer);
  void End(CommandBuffer &command_buffer, VkImageView image_view, const Walle::Extent2D &extent);

protected:
  struct ImGuiPushConstants {
    float scale_x;
    float scale_y;
    float translate_x;
    float translate_y;
  };

protected:
  void SetBuffers();
  void SetupRenderState(CommandBuffer &command_buffer, int32_t fb_width, int32_t fb_height);
  void CreateTexture();

private:
  GraphicsPipeline graphics_pipeline_;
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  ImGuiPushConstants imgui_push_constants_;
  Image image_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMGUI_RENDERER_H