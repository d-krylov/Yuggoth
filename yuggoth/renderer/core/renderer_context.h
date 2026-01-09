#ifndef YUGGOTH_RENDERER_CONTEXT_H
#define YUGGOTH_RENDERER_CONTEXT_H

#include "walle/walle.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/application/application_context.h"
#include <vector>

namespace Yuggoth {

class BufferManager;
class PipelineLibrary;
class AssetManager;

enum class ObjectMode {
  COLORED,
  TEXTURED
};

struct PushConstantMatrices {
  Matrix4f projection_ = Matrix4f(1.0f);
  Matrix4f view_ = Matrix4f(1.0f);
  Matrix4f transform_ = Matrix4f(1.0f);
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_CONTEXT_H