#include "command_buffer.h"

namespace Yuggoth {

CommandBuffer::CommandBuffer(const VkCommandPool command_pool) {
  VkCommandBufferAllocateInfo command_buffer_ai{};
  {
    command_buffer_ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_ai.commandPool = command_pool;
    command_buffer_ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_ai.commandBufferCount = 1;
  }
  VK_CHECK(vkAllocateCommandBuffers(GraphicsContext::Get()->GetDevice(), &command_buffer_ai, &command_buffer_));
}

CommandBuffer::~CommandBuffer() {
}

CommandBuffer::CommandBuffer(CommandBuffer &&other) noexcept {
  command_buffer_ = std::exchange(other.command_buffer_, VK_NULL_HANDLE);
}

CommandBuffer &CommandBuffer::operator=(CommandBuffer &&other) noexcept {
  if (this != &other) {
    command_buffer_ = std::exchange(other.command_buffer_, VK_NULL_HANDLE);
  }
  return *this;
}

const VkCommandBuffer *CommandBuffer::GetPointer() const {
  return &command_buffer_;
}

void CommandBuffer::Begin() {
  VkCommandBufferBeginInfo command_buffer_bi{};
  {
    command_buffer_bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  }
  vkBeginCommandBuffer(command_buffer_, &command_buffer_bi);
}

void CommandBuffer::Reset() {
  vkResetCommandBuffer(command_buffer_, 0);
}

void CommandBuffer::CommandBeginRendering(const VkExtent2D &extent, std::span<const VkRenderingAttachmentInfo> colors) {
  VkRenderingInfo rendering_info{};
  {
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering_info.renderArea.offset = {0, 0};
    rendering_info.renderArea.extent = extent;
    rendering_info.layerCount = 1;
    rendering_info.colorAttachmentCount = colors.size();
    rendering_info.pColorAttachments = colors.data();
    rendering_info.pDepthAttachment = nullptr;
    rendering_info.pStencilAttachment = nullptr;
  }

  vkCmdBeginRendering(command_buffer_, &rendering_info);
}

void CommandBuffer::End() {
  vkEndCommandBuffer(command_buffer_);
}

void CommandBuffer::CommandEndRendering() {
  vkCmdEndRendering(command_buffer_);
}

void CommandBuffer::CommandPipelineBarrier(DependencyMask dependency_mask, std::span<const VkBufferMemoryBarrier2> buffer_barriers,
                                           std::span<const VkImageMemoryBarrier2> image_barriers) {
  VkDependencyInfo dependency_info{};
  {
    dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.dependencyFlags = (uint32_t)dependency_mask;
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.pBufferMemoryBarriers = buffer_barriers.data();
    dependency_info.imageMemoryBarrierCount = image_barriers.size();
    dependency_info.pImageMemoryBarriers = image_barriers.data();
  }
  vkCmdPipelineBarrier2(command_buffer_, &dependency_info);
}

void CommandBuffer::CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set_number, uint32_t binding, VkBuffer buffer) {
  VkDescriptorBufferInfo descriptor_bi{};
  {
    descriptor_bi.buffer = buffer;
    descriptor_bi.offset = 0;
    descriptor_bi.range = VK_WHOLE_SIZE;
  }

  VkWriteDescriptorSet write_descriptor_set{};
  {
    write_descriptor_set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor_set.dstBinding = binding;
    write_descriptor_set.dstArrayElement = 0;
    write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write_descriptor_set.descriptorCount = 1;
    write_descriptor_set.pBufferInfo = &descriptor_bi;
  }
  vkCmdPushDescriptorSetKHR(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set_number, 1, &write_descriptor_set);
}

void CommandBuffer::CommandSetViewport(float x, float y, float width, float height) {
  VkViewport viewport{};
  {
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
  }
  vkCmdSetViewport(command_buffer_, 0, 1, &viewport);
}

void CommandBuffer::CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) {
  VkRect2D scissor;
  {
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = height;
  }
  vkCmdSetScissor(command_buffer_, 0, 1, &scissor);
}

void CommandBuffer::CommandSetCullMode(CullModeMask mode) {
  vkCmdSetCullMode(command_buffer_, mode.GetValue());
}

void CommandBuffer::CommandSetFrontFace(FrontFace front_face) {
  vkCmdSetFrontFace(command_buffer_, static_cast<VkFrontFace>(front_face));
}

void CommandBuffer::CommandEnableDepthTest(bool enabled) {
  vkCmdSetDepthTestEnable(command_buffer_, enabled);
}

void CommandBuffer::CommandEnableDepthWrite(bool enabled) {
  vkCmdSetDepthWriteEnable(command_buffer_, enabled);
}

} // namespace Yuggoth