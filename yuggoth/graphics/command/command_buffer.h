#ifndef YUGGOTH_COMMAND_BUFFER_H
#define YUGGOTH_COMMAND_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <string_view>
#include <span>

namespace Yuggoth {

class CommandBuffer {
public:
  CommandBuffer() = default;

  CommandBuffer(const VkCommandPool command_pool);

  CommandBuffer(uint32_t queue_family_index);

  ~CommandBuffer();

  CommandBuffer(const CommandBuffer &) = delete;
  CommandBuffer &operator=(const CommandBuffer &) = delete;

  CommandBuffer(CommandBuffer &&other) noexcept;
  CommandBuffer &operator=(CommandBuffer &&other) noexcept;

  static VkCommandBuffer AllocateCommandBuffer(VkCommandPool command_pool, CommandBufferLevel level);

  const VkCommandBuffer *get() const;

  VkCommandBuffer GetHandle() const;

  void Submit();
  void Reset();

  void Begin(CommandBufferUsageMask usage);
  void End();

  void CommandBeginRendering(const Extent2D &extent, std::span<const RenderingAttachmentInfo> colors, //
                             const RenderingAttachmentInfo *depth = nullptr,                          //
                             const RenderingAttachmentInfo *stencil = nullptr);

  void CommandEndRendering();

  // BARRIERS
  void CommandMemoryBarrier(PipelineStageMask2 source_stage, AccessMask2 source_access, //
                            PipelineStageMask2 destination_stage, AccessMask2 destination_access);

  void CommandPipelineBarrier(std::span<const ImageMemoryBarrier2> image_barriers, //
                              std::span<const MemoryBarrier2> memory_barriers,     //
                              std::span<const BufferMemoryBarrier2> buffer_barriers);

  // DEBUG
  void CommandBeginDebugUtilsLabel(std::string_view label_name);
  void CommandEndDebugUtilsLabel();

  // PUSH
  template <typename T> void CommandPushConstants(VkPipelineLayout layout, ShaderStageMask stage, const T &data, uint32_t offset);
  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set, uint32_t binding, VkBuffer buffer, DescriptorType descriptor_type,
                                PipelineBindPoint bind_point);
  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set, uint32_t binding, VkAccelerationStructureKHR tlas, PipelineBindPoint point);
  void CommandPushDescriptorSet(std::span<const DescriptorImageInfo> images, VkPipelineLayout layout, uint32_t set_number, uint32_t binding,
                                DescriptorType descriptor_type, PipelineBindPoint bind_point);

  // OPTIONS
  void CommandSetViewport(float x, float y, float width, float height);
  void CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
  void CommandSetCullMode(CullModeMask mode);
  void CommandSetFrontFace(FrontFace front_face);
  void CommandEnableDepthTest(bool enabled);
  void CommandEnableDepthWrite(bool enabled);
  void CommandEnableStencilTest(bool enabled);

  // BIND
  void CommandBindPipeline(const VkPipeline pipeline, PipelineBindPoint bind_point);
  void CommandBindVertexBuffer(const VkBuffer buffer, std::size_t offset);
  void CommandBindIndexBuffer(const VkBuffer buffer, std::size_t offset, IndexType index_type);

  // DRAW
  void CommandDraw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance);
  void CommandDrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance);
  void CommandDrawIndexedIndirect(VkBuffer buffer, std::size_t byte_offset, uint32_t draw_count, uint32_t stride);
  void CommandDrawMeshTasks(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z);
  void CommandDrawMeshTasksIndirect(VkBuffer buffer, std::size_t byte_offset, uint32_t draw_count, uint32_t stride);

  // COPY
  void CommandCopyBufferToImage(VkBuffer buffer, VkImage image, const Extent3D &extent);
  void CommandCopyBuffer(VkBuffer source, VkBuffer destination, const BufferCopy &buffer_copy);

  // RAY
  void CommandTraceRay(const StridedDeviceAddressRegionKHR &raygen, const StridedDeviceAddressRegionKHR &miss,
                       const StridedDeviceAddressRegionKHR &hit, const StridedDeviceAddressRegionKHR &callable, uint32_t width, uint32_t height,
                       uint32_t depth);

  // HELPERS
  void TransitionImageLayout(VkImage image, ImageLayout source_layout, ImageLayout destination_layout, PipelineStageMask2 source_stage,
                             PipelineStageMask2 destination_stage, AccessMask2 source_access, AccessMask2 destination_access,
                             const ImageSubresourceRange &subresource);

private:
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
  VkCommandPool command_pool_{VK_NULL_HANDLE};
  bool own_command_pool_ = false;
};

} // namespace Yuggoth

#include "command_buffer.ipp"

#endif // YUGGOTH_COMMAND_BUFFER_H