#include "graphics_pipeline.h"
#include <utility>

namespace Yuggoth {

VkPipelineLayout CreatePipelineLayout(std::span<const VkDescriptorSetLayout> set_layouts, std::span<const VkPushConstantRange> push_constants) {
  VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
  VkPipelineLayoutCreateInfo pipeline_layout_ci{};
  {
    pipeline_layout_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_ci.setLayoutCount = set_layouts.size();
    pipeline_layout_ci.pSetLayouts = set_layouts.data();
    pipeline_layout_ci.pushConstantRangeCount = push_constants.size();
    pipeline_layout_ci.pPushConstantRanges = push_constants.data();
  }
  VK_CHECK(vkCreatePipelineLayout(GraphicsContext::Get()->GetDevice(), &pipeline_layout_ci, nullptr, &pipeline_layout));
  return pipeline_layout;
}

auto GetVertexInputState(std::span<const VkVertexInputAttributeDescription> vertex_input_attributes) {
  VkVertexInputBindingDescription vertex_binding_description{};
  {
    vertex_binding_description.binding = 0;
    vertex_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // vertex_binding_description.stride =
  }

  VkPipelineVertexInputStateCreateInfo vertex_input_state_ci{};
  {
    vertex_input_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state_ci.vertexBindingDescriptionCount = 1;
    vertex_input_state_ci.pVertexBindingDescriptions = &vertex_binding_description;
    vertex_input_state_ci.vertexAttributeDescriptionCount = vertex_input_attributes.size();
    vertex_input_state_ci.pVertexAttributeDescriptions = vertex_input_attributes.data();
  }
  return vertex_input_state_ci;
}

VkPipeline CreateGraphicsPipeline(const GraphicsPipelineSpecification &specification, std::span<const ShaderModule> shader_modules,
                                  VkPipelineLayout pipeline_layout) {
  VkPipeline graphics_pipleine = VK_NULL_HANDLE;

  std::vector<VkShaderModuleCreateInfo> shader_modules_cis(shader_modules.size());
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_cis(shader_modules.size());

  for (auto i = 0; i < shader_modules.size(); i++) {
    VkShaderModuleCreateInfo shader_module_ci{};
    {
      shader_modules_cis[i].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      shader_modules_cis[i].codeSize = shader_modules[i].GetSize();
      shader_modules_cis[i].pCode = shader_modules[i].GetBinaryData().data();
    }

    VkPipelineShaderStageCreateInfo shader_stage_ci{};
    {
      shader_stages_cis[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shader_stages_cis[i].stage = (VkShaderStageFlagBits)shader_modules[i].GetStage();
      shader_stages_cis[i].pNext = &shader_modules_cis[i];
      shader_stages_cis[i].pName = "main";
    }
  }

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_ci{};
  {
    input_assembly_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state_ci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  }

  VkPipelineViewportStateCreateInfo viewport_state_ci{};
  {
    viewport_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_ci.viewportCount = 1;
    viewport_state_ci.scissorCount = 1;
  }

  VkPipelineRasterizationStateCreateInfo rasterization_state_ci{};
  {
    rasterization_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_ci.lineWidth = 1.0f;
    rasterization_state_ci.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization_state_ci.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterization_state_ci.depthBiasEnable = true;
  }

  VkPipelineDynamicStateCreateInfo dynamic_state_ci{};
  {
    dynamic_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_ci.dynamicStateCount = specification.dynamic_states_.size();
    dynamic_state_ci.pDynamicStates = (VkDynamicState *)specification.dynamic_states_.data();
  }

  VkPipelineRenderingCreateInfo pipeline_rendering_ci{};
  {
    pipeline_rendering_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipeline_rendering_ci.colorAttachmentCount = specification.color_formats_.size();
    pipeline_rendering_ci.pColorAttachmentFormats = (VkFormat *)specification.color_formats_.data();
    pipeline_rendering_ci.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
    pipeline_rendering_ci.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
  }

  VkPipelineMultisampleStateCreateInfo multisample_state_ci{};
  {
    multisample_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_ci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  }

  VkPipelineColorBlendStateCreateInfo color_blend_state_ci{};
  {
    color_blend_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_ci.attachmentCount = 1;
    // color_blend_state_ci.pAttachments = &color_blend_attachment_state;
  }

  VkGraphicsPipelineCreateInfo graphics_pipeline_ci{};
  {
    graphics_pipeline_ci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_ci.pNext = &pipeline_rendering_ci;
    graphics_pipeline_ci.stageCount = shader_stages_cis.size();
    graphics_pipeline_ci.pStages = shader_stages_cis.data();
    // graphics_pipeline_ci.pVertexInputState;
    graphics_pipeline_ci.pInputAssemblyState = &input_assembly_state_ci;
    graphics_pipeline_ci.pViewportState = &viewport_state_ci;
    graphics_pipeline_ci.pRasterizationState = &rasterization_state_ci;
    graphics_pipeline_ci.pMultisampleState = &multisample_state_ci;
    // graphics_pipeline_ci.pDepthStencilState;
    graphics_pipeline_ci.pColorBlendState = &color_blend_state_ci;
    graphics_pipeline_ci.pDynamicState = &dynamic_state_ci;
    graphics_pipeline_ci.layout = pipeline_layout;
  }

  VK_CHECK(vkCreateGraphicsPipelines(GraphicsContext::Get()->GetDevice(), nullptr, 1, &graphics_pipeline_ci, 0, &graphics_pipleine));
  return graphics_pipleine;
}

VkPipeline GraphicsPipeline::GetPipeline() const {
  return pipeline_;
}

VkPipelineLayout GraphicsPipeline::GetPipelineLayout() const {
  return pipeline_layout_;
}

// CONSTRUCTORS

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSpecification &specification) {
}

GraphicsPipeline::~GraphicsPipeline() {
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline &&other) noexcept {
  pipeline_ = std::exchange(other.pipeline_, VK_NULL_HANDLE);
  pipeline_layout_ = std::exchange(other.pipeline_layout_, VK_NULL_HANDLE);
}

GraphicsPipeline &GraphicsPipeline::operator=(GraphicsPipeline &&other) noexcept {
  return *this;
}

} // namespace Yuggoth