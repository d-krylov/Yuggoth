#include "pipeline_library.h"
#include "shader_library.h"
#include "yuggoth/graphics/core/graphics_specifications.h"

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
  auto square_vs = shader_library_->GetShaderModule("base/square.vert.spv");
  auto square_fs = shader_library_->GetShaderModule("base/square.frag.spv");

  auto push_descriptor_mask = DescriptorSetLayoutCreateMaskBits::E_PUSH_DESCRIPTOR_BIT;

  specification.shader_modules_ = {base_vs, base_colored_fs};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  specification.vertex_inputs_ = base_vs->GetVertexInputAttributes();
  specification.push_constants_ = ShaderModule::CreatePushConstantSpecification(specification.shader_modules_);
  graphics_pipelines_["base_colored"] = GraphicsPipeline(specification);

  specification.shader_modules_ = {base_vs, base_textured_fs};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  specification.push_constants_ = ShaderModule::CreatePushConstantSpecification(specification.shader_modules_);
  graphics_pipelines_["base_textured"] = GraphicsPipeline(specification);

  specification.shader_modules_ = {base_indirect_vs, base_indirect_fs};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  specification.push_constants_ = ShaderModule::CreatePushConstantSpecification(specification.shader_modules_);
  graphics_pipelines_["base_indirect"] = GraphicsPipeline(specification);

  specification.shader_modules_ = {mesh_ms, mesh_fs};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  specification.push_constants_ = ShaderModule::CreatePushConstantSpecification(specification.shader_modules_);
  graphics_pipelines_["mesh"] = GraphicsPipeline(specification);

  specification.shader_modules_ = {square_vs, square_fs};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  graphics_pipelines_["square"] = GraphicsPipeline(specification);

  auto ray_rgen = shader_library_->GetShaderModule("ray/mesh.rgen.spv");
  auto ray_miss = shader_library_->GetShaderModule("ray/mesh.rmiss.spv");
  auto ray_chit = shader_library_->GetShaderModule("ray/mesh.rchit.spv");

  RayTracingPipelineSpecification ray_specification;
  ray_specification.shader_groups_ = {
    {{ray_rgen}, RayTracingShaderGroupTypeKHR::E_GENERAL_KHR},            //
    {{ray_miss}, RayTracingShaderGroupTypeKHR::E_GENERAL_KHR},            //
    {{ray_chit}, RayTracingShaderGroupTypeKHR::E_TRIANGLES_HIT_GROUP_KHR} //
  };

  raytracing_pipelines_["ray"] = RayTracingPipeline(ray_specification);
}

const GraphicsPipeline &PipelineLibrary::GetPipeline(const char *name) const {
  return graphics_pipelines_.at(name);
}

const RayTracingPipeline &PipelineLibrary::GetRayTracingPipeline(const char *name) const {
  return raytracing_pipelines_.at(name);
}

} // namespace Yuggoth