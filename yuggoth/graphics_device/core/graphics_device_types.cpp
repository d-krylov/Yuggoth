#include "graphics_device_types.h"
#include "graphics_device.h"
#include <yuggoth/graphics/command/command_buffer.h>
#include <print>

namespace Yuggoth {

void BufferDeleter::operator()(Buffer *buffer) const noexcept {
  graphics_device_->GetHandlePool().buffers_.free(buffer);
}

void ImageDeleter::operator()(Image *image) const noexcept {
  graphics_device_->GetHandlePool().images_.free(image);
}

void CommandBufferDeleter::operator()(CommandBuffer *command_buffer) const noexcept {
  graphics_device_->GetHandlePool().command_buffers_.free(command_buffer);
}

} // namespace Yuggoth