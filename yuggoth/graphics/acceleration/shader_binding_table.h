#ifndef YUGGOTH_SHADER_BINDING_TABLE_H
#define YUGGOTH_SHADER_BINDING_TABLE_H

#include "yuggoth/core/tools/core_types.h"
#include "yuggoth/graphics/core/graphics_types.h"

namespace Yuggoth {

struct ShaderGroupSpecification;

class ShaderBindingTable {
public:
  ShaderBindingTable() = default;

  ShaderBindingTable(VkPipeline pipeline, uint32_t miss, uint32_t hit, uint32_t callable);

  ~ShaderBindingTable();

  ShaderBindingTable(const ShaderBindingTable &) = delete;
  ShaderBindingTable &operator=(const ShaderBindingTable &) = delete;

  ShaderBindingTable(ShaderBindingTable &&other) noexcept;
  ShaderBindingTable &operator=(ShaderBindingTable &&other) noexcept;

  const StridedDeviceAddressRegionKHR &GetRaygenRegion() const;
  const StridedDeviceAddressRegionKHR &GetMissRegion() const;
  const StridedDeviceAddressRegionKHR &GetHitRegion() const;
  const StridedDeviceAddressRegionKHR &GetCallableRegion() const;

protected:
  void CreateShaderBindingTable(VkPipeline pipeline, uint32_t miss, uint32_t hit, uint32_t callable);

  void SetRegions(uint32_t miss, uint32_t hit, uint32_t callable);

  void Swap(ShaderBindingTable &other) noexcept;

private:
  VkBuffer sbt_buffer_ = VK_NULL_HANDLE;
  VmaAllocation sbt_buffer_allocation_ = VK_NULL_HANDLE;
  StridedDeviceAddressRegionKHR raygen_sbt;
  StridedDeviceAddressRegionKHR miss_sbt_;
  StridedDeviceAddressRegionKHR hit_sbt_;
  StridedDeviceAddressRegionKHR callable_sbt_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SHADER_BINDING_TABLE_H