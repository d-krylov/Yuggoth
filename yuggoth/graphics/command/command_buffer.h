#ifndef YUGGOTH_COMMAND_BUFFER_H
#define YUGGOTH_COMMAND_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <span>

namespace Yuggoth {

class CommandBuffer {
public:
  CommandBuffer(const VkCommandPool command_pool);

  ~CommandBuffer();

  CommandBuffer(const CommandBuffer &) = delete;
  CommandBuffer &operator=(const CommandBuffer &) = delete;

  CommandBuffer(CommandBuffer &&other) noexcept;
  CommandBuffer &operator=(CommandBuffer &&other) noexcept;

  const VkCommandBuffer *GetPointer() const;

  void Begin();
  void End();

  void CommandBeginRendering(const VkExtent2D &extent, std::span<const VkRenderingAttachmentInfo> colors);
  void CommandEndRendering();

  void Reset();

  void CommandPipelineBarrier(DependencyMask dependency_mask, std::span<const VkBufferMemoryBarrier2> buffer_barriers,
                              std::span<const VkImageMemoryBarrier2> image_barriers);

  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set_number, uint32_t binding, VkBuffer buffer);

  void CommandSetViewport(float x, float y, float width, float height);
  void CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
  void CommandSetCullMode(CullModeMask mode);
  void CommandSetFrontFace(FrontFace front_face);
  void CommandEnableDepthTest(bool enabled);
  void CommandEnableDepthWrite(bool enabled);
  void CommandEnableStencilTest(bool enabled);

private:
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#include "command_buffer.ipp"

#endif // YUGGOTH_COMMAND_BUFFER_H