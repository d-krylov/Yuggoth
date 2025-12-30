#ifndef YUGGOTH_SAMPLER_H
#define YUGGOTH_SAMPLER_H

#include "yuggoth/graphics/core/graphics_specifications.h"

namespace Yuggoth {

class Sampler {
public:
  Sampler(const SamplerSpecification &specification);

  ~Sampler();

  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;

  Sampler(Sampler &&other) noexcept;
  Sampler &operator=(Sampler &&other) noexcept;

  static VkSampler CreateSampler(const SamplerSpecification &specification);

  VkSampler GetHandle() const;

private:
  VkSampler sampler_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_SAMPLER_H