#include "graphics_device.h"
#include <yuggoth/graphics/buffer/buffer.h>
#include <yuggoth/graphics/image/image.h>
#include <yuggoth/asset/model/mesh.h>
#include <yuggoth/scene/components/light.h>
#include <print>
#include <cassert>
#include <set>

namespace Yuggoth {

GraphicsDevice *GraphicsDevice::graphics_device_instance_ = nullptr;

GraphicsDevice *GraphicsDevice::Get() {
  assert(graphics_device_instance_ != nullptr);
  return graphics_device_instance_;
}

GraphicsDevice::GraphicsDevice() : resource_manager_(this) {
  CreateFrames();
  SetResourceManager();
}

GraphicsDevice::~GraphicsDevice() {
}

uint32_t FRAMES_IN_FLIGHT = 3;
uint64_t current_frame_index_ = 0;

void GraphicsDevice::CreateFrames() {
  frames_.resize(FRAMES_IN_FLIGHT);
}

void GraphicsDevice::BeginFrame() {
  current_frame_index_ = frame_index_ % FRAMES_IN_FLIGHT;
  frames_[current_frame_index_].Begin();
}

void GraphicsDevice::EndFrame() {
  frames_[current_frame_index_].End();
  frame_index_++;
}

Frame &GraphicsDevice::GetCurrentFrame() {
  return frames_[current_frame_index_];
}

void GraphicsDevice::OnResize(uint32_t width, uint32_t height) {
  GetCurrentFrame().OnResize(width, height);
}

void GraphicsDevice::SetResourceManager() {
  Walle::BufferUsageMask USAGE = Walle::BufferUsageMaskBits::E_STORAGE_BUFFER_BIT | CommonMasks::BUFFER_USAGE_SOURCES_AS;
  resource_manager_.RegisterStaticBufferAllocator<uint32_t>(
    BufferCreateInformation::CreateGPUBuffer(100_MiB, USAGE | Walle::BufferUsageMaskBits::E_INDEX_BUFFER_BIT));
  resource_manager_.RegisterStaticBufferAllocator<Vertex>(
    BufferCreateInformation::CreateGPUBuffer(100_MiB, USAGE | Walle::BufferUsageMaskBits::E_VERTEX_BUFFER_BIT));
  resource_manager_.RegisterStaticBufferAllocator<Walle::DrawIndexedIndirectCommand>(
    BufferCreateInformation::CreateCPUBuffer(10_MiB, Walle::BufferUsageMaskBits::E_INDIRECT_BUFFER_BIT));
  resource_manager_.RegisterStaticBufferAllocator<Matrix4f>(BufferCreateInformation::CreateCPUBuffer(10_MiB, Walle::BufferUsageMaskBits::E_STORAGE_BUFFER_BIT));
  resource_manager_.RegisterStaticBufferAllocator<Light>(BufferCreateInformation::CreateCPUBuffer(32_KiB, Walle::BufferUsageMaskBits::E_UNIFORM_BUFFER_BIT));
}

// Resources
UniqueBufferHandle GraphicsDevice::CreateBuffer(const BufferCreateInformation &buffer_ci) {
  auto buffer = handle_pool_.buffers_.allocate(buffer_ci);
  UniqueBufferHandle buffer_handle(buffer, BufferDeleter(this));
  return buffer_handle;
}

UniqueImageHandle GraphicsDevice::CreateImage(const ImageCreateInformation &image_ci) {
  auto image = handle_pool_.images_.allocate(image_ci, SamplerCreateInformation());
  UniqueImageHandle image_handle(image, ImageDeleter(this));
  return image_handle;
}

UniqueCommandBufferHandle GraphicsDevice::RequestCommandBuffer() {
  auto command_buffer = handle_pool_.command_buffers_.allocate();
}

HandlePool &GraphicsDevice::GetHandlePool() {
  return handle_pool_;
}

ResourceManager &GraphicsDevice::GetResourceManager() {
  return resource_manager_;
}

} // namespace Yuggoth