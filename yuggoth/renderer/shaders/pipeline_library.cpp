#include "pipeline_library.h"
#include "shader_library.h"

namespace Yuggoth {

PipelineLibrary::PipelineLibrary(ShaderLibrary *shader_library) : shader_library_(shader_library) {
  CreatePipelines();
}

void PipelineLibrary::CreatePipelines() {
  GraphicsPipelineSpecification specification;
  specification.color_formats_.emplace_back(Format::E_R8G8B8A8_UNORM);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_TEST_ENABLE);
  specification.dynamic_states_.emplace_back(DynamicState::E_DEPTH_WRITE_ENABLE);
  specification.depth_format_ = Format::E_D32_SFLOAT;
  specification.cull_mode_ = CullModeMaskBits::E_BACK_BIT;

  auto base_vs = shader_library_->GetShaderModule("base/mesh.vert.spv");
  auto base_colored_fs = shader_library_->GetShaderModule("base/mesh_colored.frag.spv");
  auto base_textured_fs = shader_library_->GetShaderModule("base/mesh_textured.frag.spv");
  auto base_indirect_vs = shader_library_->GetShaderModule("base/mesh_indirect.vert.spv");
  auto base_indirect_fs = shader_library_->GetShaderModule("base/mesh_indirect.frag.spv");
  auto mesh_ms = shader_library_->GetShaderModule("mesh/mesh.mesh.spv");
  auto mesh_fs = shader_library_->GetShaderModule("mesh/mesh.frag.spv");

  specification.shader_modules_ = {base_vs, base_colored_fs};
  graphics_pipelines_["base_colored"] = GraphicsPipeline(specification);
  specification.shader_modules_ = {base_vs, base_textured_fs};
  graphics_pipelines_["base_textured"] = GraphicsPipeline(specification);
  specification.shader_modules_ = {base_indirect_vs, base_indirect_fs};
  graphics_pipelines_["base_indirect"] = GraphicsPipeline(specification);
  specification.shader_modules_ = {mesh_ms, mesh_fs};
  graphics_pipelines_["mesh"] = GraphicsPipeline(specification);
}

const GraphicsPipeline &PipelineLibrary::GetPipeline(const char *name) const {
  return graphics_pipelines_.at(name);
}

} // namespace Yuggoth