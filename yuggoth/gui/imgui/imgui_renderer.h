#ifndef YUGGOTH_IMGUI_RENDERER_H
#define YUGGOTH_IMGUI_RENDERER_H

#include "yuggoth/graphics/pipeline/graphics_pipeline.h"
#include "yuggoth/graphics/buffer/buffer.h"

namespace Yuggoth {

class ImGuiRenderer {
public:
  ImGuiRenderer();

protected:
  struct ImGuiPushConstants {
    float scale_x;
    float scale_y;
    float translate_x;
    float translate_y;
  };

private:
  GraphicsPipeline graphics_pipeline_;
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  ImGuiPushConstants imgui_push_constants_;
};

} // namespace Yuggoth

#endif // YUGGOTH_IMGUI_RENDERER_H