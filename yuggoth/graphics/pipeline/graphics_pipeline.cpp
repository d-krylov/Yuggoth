#include "graphics_pipeline.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include <utility>
#include <print>

namespace Yuggoth {

VkPipeline CreateGraphicsPipeline(const GraphicsPipelineSpecification &specification, VkPipelineLayout pipeline_layout) {
  VkPipeline graphics_pipleine = VK_NULL_HANDLE;

  const auto &shader_modules = specification.shader_modules_;

  std::vector<ShaderModuleCreateInfo> shader_modules_cis(shader_modules.size());
  std::vector<PipelineShaderStageCreateInfo> shader_stages_cis(shader_modules.size());

  for (auto i = 0; i < specification.shader_modules_.size(); i++) {

    auto binary_data = shader_modules[i]->GetBinaryData();

    ShaderModuleCreateInfo shader_module_ci;
    shader_modules_cis[i].codeSize = binary_data.size_bytes();
    shader_modules_cis[i].pCode = reinterpret_cast<const uint32_t *>(binary_data.data());

    PipelineShaderStageCreateInfo shader_stage_ci;
    shader_stages_cis[i].stage = shader_modules[i]->GetShaderStage();
    shader_stages_cis[i].pNext = &shader_modules_cis[i];
    shader_stages_cis[i].pSpecializationInfo = nullptr;
    shader_stages_cis[i].pName = "main";
  }

  Walle::VertexInputBindingDescription vertex_binding_description;
  Walle::PipelineVertexInputStateCreateInfo vertex_input_state_ci;

  if (specification.vertex_inputs_.empty() == false) {

    const auto &back_attribute = specification.vertex_inputs_.back();

    auto stride = back_attribute.offset + Walle::GetFormatInformation(back_attribute.format).texel_block_size;

    vertex_binding_description.binding = 0;
    vertex_binding_description.inputRate = VertexInputRate::E_VERTEX;
    vertex_binding_description.stride = stride;

    vertex_input_state_ci.vertexBindingDescriptionCount = 1;
    vertex_input_state_ci.pVertexBindingDescriptions = &vertex_binding_description;
    vertex_input_state_ci.vertexAttributeDescriptionCount = specification.vertex_inputs_.size();
    vertex_input_state_ci.pVertexAttributeDescriptions = specification.vertex_inputs_.data();
  }

  PipelineInputAssemblyStateCreateInfo input_assembly_state_ci;
  input_assembly_state_ci.topology = PrimitiveTopology::E_TRIANGLE_LIST;

  PipelineViewportStateCreateInfo viewport_state_ci;

  viewport_state_ci.viewportCount = 1;
  viewport_state_ci.scissorCount = 1;

  Walle::PipelineRasterizationStateCreateInfo rasterization_state_ci;

  rasterization_state_ci.lineWidth = 1.0f;
  rasterization_state_ci.frontFace = FrontFace::E_COUNTER_CLOCKWISE;
  rasterization_state_ci.polygonMode = PolygonMode::E_FILL;
  rasterization_state_ci.cullMode = specification.cull_mode_;

  rasterization_state_ci.depthBiasEnable = false;

  Walle::PipelineDynamicStateCreateInfo dynamic_state_ci;
  dynamic_state_ci.dynamicStateCount = specification.dynamic_states_.size();
  dynamic_state_ci.pDynamicStates = specification.dynamic_states_.data();

  Walle::PipelineDepthStencilStateCreateInfo depth_stencil_state_ci;
  depth_stencil_state_ci.depthCompareOp = CompareOp::E_LESS;
  depth_stencil_state_ci.depthTestEnable = false;
  depth_stencil_state_ci.stencilTestEnable = false;

  Walle::PipelineRenderingCreateInfo pipeline_rendering_ci;
  pipeline_rendering_ci.colorAttachmentCount = specification.color_formats_.size();
  pipeline_rendering_ci.pColorAttachmentFormats = specification.color_formats_.data();
  pipeline_rendering_ci.depthAttachmentFormat = specification.depth_format_;
  pipeline_rendering_ci.stencilAttachmentFormat = specification.stencil_format_;

  Walle::PipelineMultisampleStateCreateInfo multisample_state_ci;
  multisample_state_ci.rasterizationSamples = SampleCountMaskBits::E_1_BIT;

  auto color_blend_attachment_state = GetColorBlendAttachmentState(true);

  Walle::PipelineColorBlendStateCreateInfo color_blend_state_ci;
  color_blend_state_ci.attachmentCount = 1;
  color_blend_state_ci.pAttachments = &color_blend_attachment_state;

  Walle::GraphicsPipelineCreateInfo graphics_pipeline_ci;
  graphics_pipeline_ci.pNext = &pipeline_rendering_ci;
  graphics_pipeline_ci.stageCount = shader_stages_cis.size();
  graphics_pipeline_ci.pStages = shader_stages_cis.data();
  graphics_pipeline_ci.pVertexInputState = &vertex_input_state_ci;
  graphics_pipeline_ci.pInputAssemblyState = &input_assembly_state_ci;
  graphics_pipeline_ci.pViewportState = &viewport_state_ci;
  graphics_pipeline_ci.pRasterizationState = &rasterization_state_ci;
  graphics_pipeline_ci.pMultisampleState = &multisample_state_ci;
  graphics_pipeline_ci.pDepthStencilState = &depth_stencil_state_ci;
  graphics_pipeline_ci.pColorBlendState = &color_blend_state_ci;
  graphics_pipeline_ci.pDynamicState = &dynamic_state_ci;
  graphics_pipeline_ci.layout = pipeline_layout;

  VK_CHECK(vkCreateGraphicsPipelines(GraphicsContext::Get()->GetDevice(), nullptr, 1, graphics_pipeline_ci, 0, &graphics_pipleine));
  return graphics_pipleine;
}

// CONSTRUCTORS

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSpecification &specification) {
  descriptor_set_layouts_ = CreateDescriptorSetLayouts(specification.descriptor_sets_);
  pipeline_layout_ = CreatePipelineLayout(descriptor_set_layouts_, specification.push_constants_);
  pipeline_ = CreateGraphicsPipeline(specification, pipeline_layout_);
}

Walle::PipelineBindPoint GraphicsPipeline::GetBindPoint() const {
  return Walle::PipelineBindPoint::E_GRAPHICS;
}

} // namespace Yuggoth