#ifndef YUGGOTH_PIPELINE_CREATE_INFORMATION_H
#define YUGGOTH_PIPELINE_CREATE_INFORMATION_H

#include <walle/walle.h>
#include <vector>
#include <filesystem>

namespace Yuggoth {

struct GraphicsPipelineCreateInformation {
  std::vector<Walle::DescriptorSetLayoutBinding> pool_descriptor_sets_;
  std::vector<Walle::PipelineColorBlendAttachmentState> color_blend_attachment_states_;
  std::vector<Walle::PushConstantRange> push_constants_;
  std::vector<Walle::DynamicState> dynamic_states_;
  std::vector<Walle::Format> color_formats_;
  Walle::Format depth_format_ = Walle::Format::E_UNDEFINED;
  Walle::Format stencil_format_ = Walle::Format::E_UNDEFINED;
  Walle::CullModeMask cull_mode_ = Walle::CullModeMaskBits::E_NONE;
  Walle::PrimitiveTopology primitive_topology_ = Walle::PrimitiveTopology::E_TRIANGLE_LIST;
  Walle::DescriptorSetLayoutBinding push_descriptor_set_;
};

} // namespace Yuggoth

#endif // YUGGOTH_PIPELINE_CREATE_INFORMATION_H