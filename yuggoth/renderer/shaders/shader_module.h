#ifndef YUGGOTH_SHADER_MODULE_H
#define YUGGOTH_SHADER_MODULE_H

#include "yuggoth/graphics/wrappers/graphics_structures.h"
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
  ShaderStageMask stage_;
  DescriptorType type_;
};

class ShaderModule {
public:
  ShaderModule(const std::filesystem::path &path);

  ShaderStageMaskBits GetShaderStage() const;

  std::span<const uint8_t> GetBinaryData() const;

  const std::unordered_map<std::string, PushConstantRange> &GetPushConstants() const;
  const std::unordered_map<std::string, BindingInformation> &GetDescriptorSetBindings() const;
  const std::unordered_map<std::string, VertexInputAttributeDescription> &GetVertexInputs() const;

  std::vector<VertexInputAttributeDescription> GetVertexInputAttributes() const;

  static std::vector<PushConstantRange> CreatePushConstantSpecification(std::span<const ShaderModule *const> shader_modules);
  static std::vector<DescriptorSetSpecification> CreateDescriptorSetSpecifications(std::span<const ShaderModule *const> shader_modules,
                                                                                   std::unordered_map<uint32_t, DescriptorSetLayoutCreateMask> masks);

protected:
  void ParsePushConstants(const SpvReflectShaderModule &module);
  void ParseShaderInputs(const SpvReflectShaderModule &module);
  void ParseDescriptorSetBinding(const SpvReflectShaderModule &module);

private:
  std::vector<uint8_t> spirv_;
  ShaderStageMaskBits shader_stage_;
  std::unordered_map<std::string, BindingInformation> set_bindings_;
  std::unordered_map<std::string, PushConstantRange> push_constants_;
  std::unordered_map<std::string, VertexInputAttributeDescription> inputs_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SHADER_MODULE_H