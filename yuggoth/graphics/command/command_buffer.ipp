#ifndef YUGGOTH_COMMAND_BUFFER_IPP
#define YUGGOTH_COMMAND_BUFFER_IPP

namespace Yuggoth {

template <typename T> void CommandBuffer::CommandPushConstants(VkPipelineLayout layout, Walle::ShaderStageMask stage, const T &data, uint32_t offset) {
  vkCmdPushConstants(command_buffer_, layout, stage.GetValue(), offset, sizeof(T), &data);
}

} // namespace Yuggoth

#endif // YUGGOTH_COMMAND_BUFFER_IPP