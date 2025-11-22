#ifndef YUGGOTH_SAMPLER_H
#define YUGGOTH_SAMPLER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

struct SamplerSpecification {
  Filter min_filter_ = Filter::E_LINEAR;
  Filter mag_filter_ = Filter::E_LINEAR;
  SamplerMipmapMode mipmap_mode_ = SamplerMipmapMode::E_LINEAR;
  SamplerAddressMode address_mode_ = SamplerAddressMode::E_CLAMP_TO_EDGE;
};

class Sampler {
public:
  Sampler();

  ~Sampler();

  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;

  Sampler(Sampler &&other) noexcept;
  Sampler &operator=(Sampler &&other) noexcept;

  static VkSampler CreateVulkanSampler(const SamplerSpecification &specification);

  VkSampler GetHandle() const;

private:
  VkSampler sampler_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_SAMPLER_H