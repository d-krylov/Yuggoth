#ifndef YUGGOTH_RENDERER_CONTEXT_H
#define YUGGOTH_RENDERER_CONTEXT_H

#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/graphics/core/graphics_structures.h"
#include <vector>

namespace Yuggoth {

class BufferManager;
class PipelineLibrary;

enum class ObjectMode {
  COLORED,
  TEXTURED
};

struct RendererContext {
  PipelineLibrary *pipeline_library_ = nullptr;
  BufferManager *buffer_manager_ = nullptr;
};

struct PushConstantMatrices {
  Matrix4f projection_ = Matrix4f(1.0f);
  Matrix4f view_ = Matrix4f(1.0f);
  Matrix4f transform_ = Matrix4f(1.0f);
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_CONTEXT_H