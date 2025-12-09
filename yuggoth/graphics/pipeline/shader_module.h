#ifndef YUGGOTH_SHADER_MODULE_H
#define YUGGOTH_SHADER_MODULE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <filesystem>
#include <map>

namespace Yuggoth {

using DescriptorSetLayoutBindingMap = std::map<uint32_t, std::vector<DescriptorSetLayoutBinding>>;

class ShaderModule {
public:
  ShaderModule(const std::filesystem::path &shader_path);

  ShaderStageMaskBits GetStage() const;
  std::span<const uint32_t> GetBinaryData() const;
  std::size_t GetSize() const;

  std::span<const VertexInputAttributeDescription> GetInputDescriptions() const;
  std::span<const PushConstantRange> GetPushConstants() const;

  const DescriptorSetLayoutBindingMap &GetPushDescriptorSetLayoutBindings() const;
  const DescriptorSetLayoutBindingMap &GetPoolDescriptorSetLayoutBindings() const;

private:
  std::vector<uint32_t> spirv_;
  ShaderStageMaskBits shader_stage_;
  DescriptorSetLayoutBindingMap push_descriptor_set_bindings_;
  DescriptorSetLayoutBindingMap pool_descriptor_set_bindings_;
  std::vector<PushConstantRange> push_constants_;
  std::vector<VertexInputAttributeDescription> vertex_input_descriptions_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SHADER_MODULE_H