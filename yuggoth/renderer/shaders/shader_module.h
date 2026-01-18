#ifndef YUGGOTH_SHADER_MODULE_H
#define YUGGOTH_SHADER_MODULE_H

#include "yuggoth/graphics/core/graphics_types.h"
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>

struct SpvReflectShaderModule;

namespace Yuggoth {

struct DescriptorSetSpecification;

struct BindingInformation {
  uint32_t set_;
  uint32_t binding_;
  uint32_t count_;
  Walle::ShaderStageMask stage_;
  Walle::DescriptorType type_;
};

class ShaderModule {
public:
  ShaderModule(const std::filesystem::path &path);

  Walle::ShaderStageMaskBits GetShaderStage() const;

  std::span<const uint8_t> GetBinaryData() const;

  const std::unordered_map<std::string, BindingInformation> &GetDescriptorSetBindings() const;
  const std::unordered_map<std::string, Walle::PushConstantRange> &GetPushConstants() const;
  const std::unordered_map<std::string, Walle::VertexInputAttributeDescription> &GetVertexInputs() const;

  std::vector<Walle::VertexInputAttributeDescription> GetVertexInputAttributes() const;

  static std::vector<Walle::PushConstantRange> CreatePushConstantSpecification(std::span<const ShaderModule *const> shader_modules);
  static std::vector<DescriptorSetSpecification> CreateDescriptorSetSpecifications(std::span<const ShaderModule *const> shader_modules,
                                                                                   std::unordered_map<uint32_t, Walle::DescriptorSetLayoutCreateMask> masks);

protected:
  void ParsePushConstants(const SpvReflectShaderModule &module);
  void ParseShaderInputs(const SpvReflectShaderModule &module);
  void ParseDescriptorSetBinding(const SpvReflectShaderModule &module);

private:
  std::vector<uint8_t> spirv_;
  Walle::ShaderStageMaskBits shader_stage_;
  std::unordered_map<std::string, BindingInformation> set_bindings_;
  std::unordered_map<std::string, Walle::PushConstantRange> push_constants_;
  std::unordered_map<std::string, Walle::VertexInputAttributeDescription> inputs_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SHADER_MODULE_H