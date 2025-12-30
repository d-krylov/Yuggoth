#include "sampler.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"

namespace Yuggoth {

Sampler::Sampler(const SamplerSpecification &specification) {
}

Sampler::~Sampler() {
}

Sampler::Sampler(Sampler &&other) noexcept {
  sampler_ = std::exchange(other.sampler_, VK_NULL_HANDLE);
}

Sampler &Sampler::operator=(Sampler &&other) noexcept {
  std::swap(sampler_, other.sampler_);
  return *this;
}

VkSampler Sampler::GetHandle() const {
  return sampler_;
}

VkSampler Sampler::CreateSampler(const SamplerSpecification &specification) {
  SamplerCreateInfo sampler_ci;
  sampler_ci.magFilter = specification.min_filter_;
  sampler_ci.minFilter = specification.mag_filter_;
  sampler_ci.mipmapMode = specification.mipmap_mode_;
  sampler_ci.addressModeU = specification.address_mode_u_;
  sampler_ci.addressModeV = specification.address_mode_v_;
  sampler_ci.addressModeW = specification.address_mode_w_;
  sampler_ci.mipLodBias = 0.0f;
  sampler_ci.anisotropyEnable = false;
  sampler_ci.maxAnisotropy = 1.0f;
  sampler_ci.compareEnable = false;
  sampler_ci.compareOp = CompareOp::E_ALWAYS;
  sampler_ci.minLod = 0.0f;
  sampler_ci.maxLod = 16.0f;
  sampler_ci.borderColor = BorderColor::E_INT_OPAQUE_BLACK;

  VkSampler sampler = VK_NULL_HANDLE;
  VK_CHECK(vkCreateSampler(GraphicsContext::Get()->GetDevice(), sampler_ci, nullptr, &sampler));
  return sampler;
}

} // namespace Yuggoth