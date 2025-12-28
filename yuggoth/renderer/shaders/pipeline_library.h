#ifndef YUGGOTH_PIPELINE_LIBRARY_H
#define YUGGOTH_PIPELINE_LIBRARY_H

#include <unordered_map>
#include "yuggoth/graphics/pipeline/graphics_pipeline.h"

namespace Yuggoth {

class ShaderLibrary;

class PipelineLibrary {
public:
  PipelineLibrary(ShaderLibrary *shader_library);

  const GraphicsPipeline &GetPipeline(const char *name) const;

protected:
  void CreatePipelines();

private:
  ShaderLibrary *shader_library_ = nullptr;
  std::unordered_map<std::string, GraphicsPipeline> graphics_pipelines_;
};

} // namespace Yuggoth

#endif // YUGGOTH_PIPELINE_LIBRARY_H