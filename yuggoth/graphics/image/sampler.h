#ifndef YUGGOTH_SAMPLER_H
#define YUGGOTH_SAMPLER_H

#include "image_create_information.h"

namespace Yuggoth {

class Sampler {
public:
  Sampler(const SamplerCreateInformation &sampler_ci);

  ~Sampler();

  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;

  Sampler(Sampler &&other) noexcept;
  Sampler &operator=(Sampler &&other) noexcept;

  static VkSampler CreateSampler(const SamplerCreateInformation &sampler_ci);

  VkSampler GetHandle() const;

private:
  VkSampler sampler_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_SAMPLER_H