
#ifndef YUGGOTH_PIPELINE_H
#define YUGGOTH_PIPELINE_H

#include "yuggoth/renderer/shaders/shader_module.h"

namespace Yuggoth {

VkPipelineLayout CreatePipelineLayout(std::span<const VkDescriptorSetLayout> set_layouts, std::span<const PushConstantRange> push_constants);
std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(std::span<const DescriptorSetSpecification> specifications);

class Pipeline {
public:
  Pipeline() = default;

  ~Pipeline();

  Pipeline(const Pipeline &) = delete;
  Pipeline &operator=(const Pipeline &) = delete;

  Pipeline(Pipeline &&other) noexcept;
  Pipeline &operator=(Pipeline &&other) noexcept;

  VkPipeline GetHandle() const;
  VkPipelineLayout GetPipelineLayout() const;

  virtual Walle::PipelineBindPoint GetBindPoint() const = 0;

protected:
  VkPipeline pipeline_{VK_NULL_HANDLE};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_PIPELINE_H
