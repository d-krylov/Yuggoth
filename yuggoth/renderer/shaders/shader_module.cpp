#include <SPIRV-Reflect/spirv_reflect.h>
#include "yuggoth/core/tools/type_tools.h"
#include "yuggoth/core/tools/filesystem.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include "shader_module.h"
#include <algorithm>
#include <print>
#include <span>

namespace Yuggoth {

void MergeDescriptorSetBindings(std::unordered_map<std::string, BindingInformation> &destination,
                                const std::unordered_map<std::string, BindingInformation> &source) {
  for (const auto &[name, binding_information] : source) {
    if (auto it = destination.find(name); it == destination.end()) {
      destination.emplace(name, binding_information);
    } else {
      auto &descriptor_binding = it->second;
      descriptor_binding.stage_ |= binding_information.stage_;
    }
  }
}

void MergePushConstants(std::unordered_map<std::string, PushConstantRange> &destination, const std::unordered_map<std::string, PushConstantRange> &source) {
  for (const auto &[name, range] : source) {
    if (auto it = destination.find(name); it == destination.end()) {
      destination.emplace(name, range);
    } else {
      auto &existing = it->second;
      existing.stageFlags |= range.stageFlags;
    }
  }
}

Walle::VertexInputAttributeDescription ToVertexInputAttributeDescription(const SpvReflectInterfaceVariable *spv_interface, uint32_t offset) {
  Walle::VertexInputAttributeDescription out;
  auto format = static_cast<Walle::Format>(spv_interface->format);
  out.location = spv_interface->location;
  out.binding = 0;
  out.format = format;
  out.offset = offset;
  return out;
}

BindingInformation ToBindingInformation(const SpvReflectDescriptorBinding *spv_set_binding, uint32_t set, ShaderStageMask stage) {
  BindingInformation out;
  auto descriptor_type = static_cast<DescriptorType>(spv_set_binding->descriptor_type);
  out.binding_ = spv_set_binding->binding;
  out.count_ = spv_set_binding->count;
  out.type_ = descriptor_type;
  out.stage_ = stage;
  out.set_ = set;
  return out;
}

PushConstantRange ToPushConstantRange(const SpvReflectBlockVariable *spv_block, ShaderStageMask stage) {
  PushConstantRange out;
  out.size = spv_block->size;
  out.offset = spv_block->absolute_offset;
  out.stageFlags = stage;
  return out;
}

void ShaderModule::ParseShaderInputs(const SpvReflectShaderModule &module) {
  auto offset = 0;
  auto inputs = Enumerate<SpvReflectInterfaceVariable *>(spvReflectEnumerateInputVariables, &module);
  std::ranges::sort(inputs, std::less(), &SpvReflectInterfaceVariable::location);
  for (const auto &input : inputs) {
    if ((input->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) == 0) {
      auto vertex_input_description = ToVertexInputAttributeDescription(input, offset);
      offset += GetFormatInformation(vertex_input_description.format).texel_block_size;
      inputs_[input->name] = vertex_input_description;
    }
  }
}

void ShaderModule::ParseDescriptorSetBinding(const SpvReflectShaderModule &module) {
  auto spv_descriptor_sets = Enumerate<SpvReflectDescriptorSet *>(spvReflectEnumerateDescriptorSets, &module);
  for (const auto &spv_descriptor_set : spv_descriptor_sets) {
    std::span<SpvReflectDescriptorBinding *> spv_set_bindings(spv_descriptor_set->bindings, spv_descriptor_set->binding_count);
    for (const auto &spv_binding : spv_set_bindings) {
      auto binding = ToBindingInformation(spv_binding, spv_descriptor_set->set, shader_stage_);
      std::string name = spv_binding->name;
      if (name.empty()) {
        name = spv_binding->type_description->type_name;
      }
      set_bindings_[name] = binding;
    }
  }
}

void ShaderModule::ParsePushConstants(const SpvReflectShaderModule &module) {
  auto spv_push_constants = Enumerate<SpvReflectBlockVariable *>(spvReflectEnumeratePushConstantBlocks, &module);
  for (const auto &spv_block : spv_push_constants) {
    auto push_constants = ToPushConstantRange(spv_block, shader_stage_);
    push_constants_[spv_block->name] = push_constants;
  }
}

ShaderModule::ShaderModule(const std::filesystem::path &path) {
  SpvReflectShaderModule spv_module;
  spirv_ = FileSystem::ReadBinaryFile(path);
  auto status = spvReflectCreateShaderModule(spirv_.size(), spirv_.data(), &spv_module);
  shader_stage_ = static_cast<ShaderStageMaskBits>(spv_module.shader_stage);
  ParsePushConstants(spv_module);
  ParseDescriptorSetBinding(spv_module);
  ParseShaderInputs(spv_module);
}

std::vector<DescriptorSetSpecification> ShaderModule::CreateDescriptorSetSpecifications(std::span<const ShaderModule *const> shader_modules,
                                                                                        std::unordered_map<uint32_t, DescriptorSetLayoutCreateMask> masks) {
  std::unordered_map<uint32_t, DescriptorSetSpecification> set_specifications;
  std::unordered_map<std::string, BindingInformation> descriptor_sets_bindings;
  for (const auto *shader_module : shader_modules) {
    MergeDescriptorSetBindings(descriptor_sets_bindings, shader_module->GetDescriptorSetBindings());
  }

  std::unordered_map<uint32_t, std::vector<BindingInformation>> descriptor_sets;
  for (const auto &[name, binding] : descriptor_sets_bindings) {
    descriptor_sets[binding.set_].emplace_back(binding);
  }

  std::vector<DescriptorSetSpecification> out;

  for (const auto &[set, bindings] : descriptor_sets) {
    DescriptorSetSpecification specification;
    specification.descriptor_set_create_mask_ = masks[set];

    for (const auto &binding : bindings) {
      Walle::DescriptorSetLayoutBinding descriptor_set_binding;
      descriptor_set_binding.binding = binding.binding_;
      descriptor_set_binding.descriptorType = binding.type_;
      descriptor_set_binding.descriptorCount = binding.count_;
      descriptor_set_binding.stageFlags = binding.stage_;

      specification.descriptor_set_bindings_.emplace_back(descriptor_set_binding);
    }

    out.emplace_back(std::move(specification));
  }

  return out;
}

std::vector<VertexInputAttributeDescription> ShaderModule::GetVertexInputAttributes() const {
  std::vector<VertexInputAttributeDescription> out;
  for (const auto &[name, attribute] : inputs_) {
    out.emplace_back(attribute);
  }
  std::ranges::sort(out, std::less(), &VertexInputAttributeDescription::location);
  return out;
}

std::vector<PushConstantRange> ShaderModule::CreatePushConstantSpecification(std::span<const ShaderModule *const> shader_modules) {
  std::vector<PushConstantRange> out;
  std::unordered_map<std::string, PushConstantRange> push_constants;
  for (const auto *shader_module : shader_modules) {
    MergePushConstants(push_constants, shader_module->GetPushConstants());
  }

  for (const auto &[name, range] : push_constants) {
    out.emplace_back(range);
  }

  return out;
}

// GETTERS
ShaderStageMaskBits ShaderModule::GetShaderStage() const {
  return shader_stage_;
}

const std::unordered_map<std::string, BindingInformation> &ShaderModule::GetDescriptorSetBindings() const {
  return set_bindings_;
}

const std::unordered_map<std::string, VertexInputAttributeDescription> &ShaderModule::GetVertexInputs() const {
  return inputs_;
}

const std::unordered_map<std::string, PushConstantRange> &ShaderModule::GetPushConstants() const {
  return push_constants_;
}

std::span<const uint8_t> ShaderModule::GetBinaryData() const {
  return spirv_;
}

} // namespace Yuggoth