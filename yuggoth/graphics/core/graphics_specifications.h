#ifndef YUGGOTH_GRAPHICS_SPECIFICATIONS_H
#define YUGGOTH_GRAPHICS_SPECIFICATIONS_H

#include "walle/vulkan_structures.h"
#include <vector>

namespace Yuggoth {

class ShaderModule;

struct SamplerSpecification {
  Walle::Filter min_filter_ = Walle::Filter::E_LINEAR;
  Walle::Filter mag_filter_ = Walle::Filter::E_LINEAR;
  Walle::SamplerMipmapMode mipmap_mode_ = Walle::SamplerMipmapMode::E_LINEAR;
  Walle::SamplerAddressMode address_mode_u_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
  Walle::SamplerAddressMode address_mode_v_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
  Walle::SamplerAddressMode address_mode_w_ = Walle::SamplerAddressMode::E_CLAMP_TO_EDGE;
};

struct DescriptorSetSpecification {
  std::vector<Walle::DescriptorSetLayoutBinding> descriptor_set_bindings_;
  Walle::DescriptorSetLayoutCreateMask descriptor_set_create_mask_;
};

struct GraphicsPipelineSpecification {
  std::vector<const ShaderModule *> shader_modules_;
  std::vector<Walle::DynamicState> dynamic_states_{Walle::DynamicState::E_VIEWPORT, Walle::DynamicState::E_SCISSOR};
  std::vector<Walle::VertexInputAttributeDescription> vertex_inputs_;
  std::vector<DescriptorSetSpecification> descriptor_sets_;
  std::vector<Walle::PushConstantRange> push_constants_;
  std::vector<Walle::Format> color_formats_;
  Walle::Format depth_format_ = Walle::Format::E_UNDEFINED;
  Walle::Format stencil_format_ = Walle::Format::E_UNDEFINED;
  Walle::CullModeMask cull_mode_ = Walle::CullModeMaskBits::E_NONE;
};

struct ShaderGroup {
  std::vector<const ShaderModule *> shader_modules_;
  Walle::RayTracingShaderGroupTypeKHR group_type_;
};

struct RayTracingPipelineSpecification {
  std::vector<ShaderGroup> shader_groups_;
  uint32_t maximum_recursion_depth_ = 1;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_SPECIFICATIONS_H