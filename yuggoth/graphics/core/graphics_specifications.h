#ifndef YUGGOTH_GRAPHICS_SPECIFICATIONS_H
#define YUGGOTH_GRAPHICS_SPECIFICATIONS_H

#include "yuggoth/graphics/wrappers/graphics_structures.h"
#include <vector>

namespace Yuggoth {

class ShaderModule;

struct ImageSpecification {
  Format format_;
  Extent3D extent_{0, 0, 0};
  uint32_t levels_{1};
  uint32_t layers_{1};
  ImageUsageMask usage_;
};

struct SamplerSpecification {
  Filter min_filter_ = Filter::E_LINEAR;
  Filter mag_filter_ = Filter::E_LINEAR;
  SamplerMipmapMode mipmap_mode_ = SamplerMipmapMode::E_LINEAR;
  SamplerAddressMode address_mode_u_ = SamplerAddressMode::E_CLAMP_TO_EDGE;
  SamplerAddressMode address_mode_v_ = SamplerAddressMode::E_CLAMP_TO_EDGE;
  SamplerAddressMode address_mode_w_ = SamplerAddressMode::E_CLAMP_TO_EDGE;
};

struct GraphicsPipelineSpecification {
  std::vector<const ShaderModule *> shader_modules_;
  std::vector<DynamicState> dynamic_states_{DynamicState::E_VIEWPORT, DynamicState::E_SCISSOR};
  std::vector<Format> color_formats_;
  Format depth_format_ = Format::E_UNDEFINED;
  Format stencil_format_ = Format::E_UNDEFINED;
  CullModeMask cull_mode_ = CullModeMaskBits::E_NONE;
};

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_SPECIFICATIONS_H