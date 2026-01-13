#include "sampler.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"

namespace Yuggoth {

Sampler::Sampler(const SamplerCreateInformation &specification) {
  CreateSampler(specification);
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

VkSampler Sampler::CreateSampler(const SamplerCreateInformation &sampler_create_information) {
  Walle::SamplerCreateInfo sampler_ci;
  sampler_ci.magFilter = sampler_create_information.min_filter_;
  sampler_ci.minFilter = sampler_create_information.mag_filter_;
  sampler_ci.mipmapMode = sampler_create_information.mipmap_mode_;
  sampler_ci.addressModeU = sampler_create_information.address_mode_u_;
  sampler_ci.addressModeV = sampler_create_information.address_mode_v_;
  sampler_ci.addressModeW = sampler_create_information.address_mode_w_;
  sampler_ci.mipLodBias = 0.0f;
  sampler_ci.anisotropyEnable = false;
  sampler_ci.maxAnisotropy = 1.0f;
  sampler_ci.compareEnable = false;
  sampler_ci.compareOp = Walle::CompareOp::E_ALWAYS;
  sampler_ci.minLod = 0.0f;
  sampler_ci.maxLod = 16.0f;
  sampler_ci.borderColor = Walle::BorderColor::E_INT_OPAQUE_BLACK;

  VkSampler sampler = VK_NULL_HANDLE;
  VK_CHECK(vkCreateSampler(GraphicsContext::Get()->GetDevice(), sampler_ci, nullptr, &sampler));
  return sampler;
}

} // namespace Yuggoth