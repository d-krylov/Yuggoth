#include "ray_tracing_pipeline.h"
#include "yuggoth/graphics/core/graphics_types.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

void SetShaderGroupCreateInformation(RayTracingShaderGroupCreateInfoKHR &shader_group, ShaderStageMaskBits shader_stage, uint32_t shader_index) {
  switch (shader_stage) {
  case ShaderStageMaskBits::E_RAYGEN_BIT_KHR:
  case ShaderStageMaskBits::E_MISS_BIT_KHR:
  case ShaderStageMaskBits::E_CALLABLE_BIT_KHR: shader_group.generalShader = shader_index; break;
  case ShaderStageMaskBits::E_ANY_HIT_BIT_KHR: shader_group.anyHitShader = shader_index; break;
  case ShaderStageMaskBits::E_CLOSEST_HIT_BIT_KHR: shader_group.closestHitShader = shader_index; break;
  case ShaderStageMaskBits::E_INTERSECTION_BIT_KHR: shader_group.intersectionShader = shader_index; break;
  default: break;
  }
}

auto GetRayTracingShaderGroups(std::span<const ShaderGroup> shader_groups, std::span<const ShaderModule *> shader_modules) {
  std::vector<RayTracingShaderGroupCreateInfoKHR> shader_group_cis;
  auto shader_index = 0;
  for (const auto &shader_group : shader_groups) {
    auto &shader_group_ci = shader_group_cis.emplace_back();
    shader_group_ci.generalShader = VK_SHADER_UNUSED_KHR;
    shader_group_ci.closestHitShader = VK_SHADER_UNUSED_KHR;
    shader_group_ci.anyHitShader = VK_SHADER_UNUSED_KHR;
    shader_group_ci.intersectionShader = VK_SHADER_UNUSED_KHR;
    shader_group_ci.type = shader_group.group_type_;
    ShaderStageMask shader_stage_mask;
    for (const auto &shader_module : shader_group.shader_modules_) {
      SetShaderGroupCreateInformation(shader_group_ci, shader_module->GetShaderStage(), shader_index++);
      shader_stage_mask |= shader_module->GetShaderStage();
    }
  }
  return shader_group_cis;
}

VkPipeline CreateRayTracingPipeline(const RayTracingPipelineSpecification &specification, std::span<const ShaderModule *> shader_modules,
                                    VkPipelineLayout pipeline_layout) {

  std::vector<ShaderModuleCreateInfo> shader_modules_cis(shader_modules.size());
  std::vector<PipelineShaderStageCreateInfo> shader_stages_cis(shader_modules.size());

  for (auto i = 0; i < shader_modules.size(); i++) {
    auto spirv = shader_modules[i]->GetBinaryData();
    shader_modules_cis[i].codeSize = spirv.size();
    shader_modules_cis[i].pCode = reinterpret_cast<const uint32_t *>(spirv.data());
    shader_stages_cis[i].stage = shader_modules[i]->GetShaderStage();
    shader_stages_cis[i].pNext = &shader_modules_cis[i];
    shader_stages_cis[i].pName = "main";
  }

  auto ray_tracing_shader_group_cis = GetRayTracingShaderGroups(specification.shader_groups_, shader_modules);

  RayTracingPipelineCreateInfoKHR pipeline_ci;
  pipeline_ci.stageCount = shader_stages_cis.size();
  pipeline_ci.pStages = shader_stages_cis.data();
  pipeline_ci.groupCount = ray_tracing_shader_group_cis.size();
  pipeline_ci.pGroups = ray_tracing_shader_group_cis.data();
  pipeline_ci.maxPipelineRayRecursionDepth = specification.maximum_recursion_depth_;
  pipeline_ci.pDynamicState = nullptr;
  pipeline_ci.layout = pipeline_layout;

  VkPipeline pipeline = VK_NULL_HANDLE;
  VK_CHECK(vkCreateRayTracingPipelinesKHR(GraphicsContext::Get()->GetDevice(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, pipeline_ci, nullptr, &pipeline));
  return pipeline;
}

RayTracingPipeline::RayTracingPipeline(const RayTracingPipelineSpecification &specification) {
  std::vector<const ShaderModule *> shader_modules;
  for (const auto &shader_group : specification.shader_groups_) {
    shader_modules.insert(shader_modules.end(), shader_group.shader_modules_.begin(), shader_group.shader_modules_.end());
  }

  auto push_descriptor_mask = DescriptorSetLayoutCreateMaskBits::E_PUSH_DESCRIPTOR_BIT;
  auto descriptor_set_specifications = ShaderModule::CreateDescriptorSetSpecifications(shader_modules, {{0, push_descriptor_mask}});
  auto push_constants_specifications = ShaderModule::CreatePushConstantSpecification(shader_modules);

  descriptor_set_layouts_ = CreateDescriptorSetLayouts(descriptor_set_specifications);
  pipeline_layout_ = CreatePipelineLayout(descriptor_set_layouts_, push_constants_specifications);

  ray_tracing_pipeline_ = CreateRayTracingPipeline(specification, shader_modules, pipeline_layout_);
}

RayTracingPipeline::~RayTracingPipeline() {
}

RayTracingPipeline::RayTracingPipeline(RayTracingPipeline &&other) noexcept {
  ray_tracing_pipeline_ = std::exchange(other.ray_tracing_pipeline_, VK_NULL_HANDLE);
  pipeline_layout_ = std::exchange(other.pipeline_layout_, VK_NULL_HANDLE);
}

RayTracingPipeline &RayTracingPipeline::operator=(RayTracingPipeline &&other) noexcept {
  std::swap(ray_tracing_pipeline_, other.ray_tracing_pipeline_);
  std::swap(pipeline_layout_, other.pipeline_layout_);
  return *this;
}

VkPipeline RayTracingPipeline::GetHandle() const {
  return ray_tracing_pipeline_;
}

VkPipelineLayout RayTracingPipeline::GetPipelineLayout() const {
  return pipeline_layout_;
}

} // namespace Yuggoth