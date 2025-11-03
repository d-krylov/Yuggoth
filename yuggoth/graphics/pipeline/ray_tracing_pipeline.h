#ifndef YUGGOTH_RAY_TRACING_PIPELINE_H
#define YUGGOTH_RAY_TRACING_PIPELINE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

class RayTracingPipeline {
public:
  RayTracingPipeline();
  ~RayTracingPipeline();

protected:
  void Create();

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_RAY_TRACING_PIPELINE_H