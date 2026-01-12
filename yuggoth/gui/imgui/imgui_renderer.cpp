#include "imgui_renderer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/core/tools/include/filesystem.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/presentation/swapchain.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include <algorithm>
#include <print>

namespace Yuggoth {

ImGuiRenderer::ImGuiRenderer(Format color_format) {
  auto shaders_root = FileSystem::GetYuggothShaderBinaryDirectory();

  auto push_descriptor_mask = DescriptorSetLayoutCreateMaskBits::E_PUSH_DESCRIPTOR_BIT;

  ShaderModule gui_vert(shaders_root / "gui" / "gui.vert.spv");
  ShaderModule gui_frag(shaders_root / "gui" / "gui.frag.spv");
  GraphicsPipelineSpecification specification;
  specification.color_formats_ = {color_format};
  specification.shader_modules_ = {&gui_vert, &gui_frag};
  specification.descriptor_sets_ = ShaderModule::CreateDescriptorSetSpecifications(specification.shader_modules_, {{0, push_descriptor_mask}});
  specification.push_constants_ = ShaderModule::CreatePushConstantSpecification(specification.shader_modules_);
  specification.vertex_inputs_ = gui_vert.GetVertexInputAttributes();
  graphics_pipeline_ = GraphicsPipeline(specification);

  vertex_buffer_ = Buffer(BufferCreateInformation::CreateCPUBuffer(10_MiB, Walle::BufferUsageMaskBits::E_VERTEX_BUFFER_BIT, true));
  index_buffer_ = Buffer(BufferCreateInformation::CreateCPUBuffer(10_MiB, Walle::BufferUsageMaskBits::E_INDEX_BUFFER_BIT, true));

  CreateTexture();
}

void ImGuiRenderer::SetBuffers() {
  auto draw_data = ImGui::GetDrawData();
  std::size_t vbo_offset = 0, ibo_offset = 0;
  auto vertex_buffer_memory = vertex_buffer_.GetMappedAs<ImDrawVert>();
  auto index_buffer_memory = index_buffer_.GetMappedAs<ImDrawIdx>();
  for (const auto &commands : draw_data->CmdLists) {
    std::span<ImDrawVert> vertices(commands->VtxBuffer.Data, commands->VtxBuffer.Size);
    std::span<ImDrawIdx> indices(commands->IdxBuffer.Data, commands->IdxBuffer.Size);
    std::ranges::copy(vertices, vertex_buffer_memory.begin() + vbo_offset);
    std::ranges::copy(indices, index_buffer_memory.begin() + ibo_offset);
    vbo_offset += vertices.size();
    ibo_offset += indices.size();
  }
}

void ImGuiRenderer::SetupRenderState(CommandBuffer &command_buffer, int32_t fb_width, int32_t fb_height) {
  auto &io = ImGui::GetIO();
  auto draw_data = ImGui::GetDrawData();

  command_buffer.CommandBindPipeline(graphics_pipeline_.GetHandle(), PipelineBindPoint::E_GRAPHICS);

  auto index_type = sizeof(ImDrawIdx) == 2 ? IndexType::E_UINT16 : IndexType::E_UINT32;

  if (draw_data->TotalVtxCount > 0) {
    command_buffer.CommandBindVertexBuffer(vertex_buffer_.GetHandle(), 0);
    command_buffer.CommandBindIndexBuffer(index_buffer_.GetHandle(), 0, index_type);
  }

  command_buffer.CommandSetViewport(0.0f, 0.0f, fb_width, fb_height);

  imgui_push_constants_.scale_x = 2.0f / draw_data->DisplaySize.x;
  imgui_push_constants_.scale_y = 2.0f / draw_data->DisplaySize.y;
  imgui_push_constants_.translate_x = -1.0f - draw_data->DisplayPos.x * imgui_push_constants_.scale_x,
  imgui_push_constants_.translate_y = -1.0f - draw_data->DisplayPos.y * imgui_push_constants_.scale_y;

  command_buffer.CommandPushConstants(graphics_pipeline_.GetPipelineLayout(), ShaderStageMaskBits::E_VERTEX_BIT, imgui_push_constants_, 0);
}

void ImGuiRenderer::RenderDrawData(CommandBuffer &command_buffer) {
  auto draw_data = ImGui::GetDrawData();
  auto framebuffer_w = static_cast<int32_t>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
  auto framebuffer_h = static_cast<int32_t>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);

  if (framebuffer_w <= 0 || framebuffer_h <= 0) {
    return;
  }

  if (draw_data->Textures != nullptr) {
    for (auto *texture : *draw_data->Textures) {
      if (texture->Status != ImTextureStatus_OK) {
      }
    }
  }

  if (draw_data->TotalVtxCount > 0) {
    SetBuffers();
  }

  SetupRenderState(command_buffer, framebuffer_w, framebuffer_h);

  auto clip_offset = draw_data->DisplayPos;
  auto clip_scale = draw_data->FramebufferScale;

  auto global_vertex_offset = 0;
  auto global_index_offset = 0;

  Image *previous_image = nullptr;

  for (const auto commands : draw_data->CmdLists) {
    for (const auto command : commands->CmdBuffer) {
      if (command.UserCallback != nullptr) {
        // Not implemented
      } else {

        Vector2f clip_min((command.ClipRect.x - clip_offset.x) * clip_scale.x, (command.ClipRect.y - clip_offset.y) * clip_scale.y);
        Vector2f clip_max((command.ClipRect.z - clip_offset.x) * clip_scale.x, (command.ClipRect.w - clip_offset.y) * clip_scale.y);

        clip_min = glm::max(Vector2f(0.0f), clip_min);
        clip_max = glm::min(clip_max, Vector2f(framebuffer_w, framebuffer_h));

        if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) {
          continue;
        }

        command_buffer.CommandSetScissor(clip_min.x, clip_min.y, clip_max.x - clip_min.x, clip_max.y - clip_min.y);

        auto first_index = command.IdxOffset + global_index_offset;
        auto vertex_offset = command.VtxOffset + global_vertex_offset;

        Image *image = reinterpret_cast<Image *>(command.GetTexID());

        if (image != previous_image) {
          std::array descriptors = {image->GetDescriptor()};

          command_buffer.CommandPushDescriptorSet(descriptors, graphics_pipeline_.GetPipelineLayout(), 0, 0, DescriptorType::E_COMBINED_IMAGE_SAMPLER,
                                                  PipelineBindPoint::E_GRAPHICS);
        }

        previous_image = image;
        command_buffer.CommandDrawIndexed(command.ElemCount, 1, first_index, vertex_offset, 0);
      }
    }
    global_index_offset += commands->IdxBuffer.Size;
    global_vertex_offset += commands->VtxBuffer.Size;
  }
  command_buffer.CommandSetScissor(0, 0, framebuffer_w, framebuffer_h);
}

void ImGuiRenderer::CreateTexture() {
  auto &io = ImGui::GetIO();
  uint8_t *pixels{nullptr};
  int32_t width, height, channels;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &channels);
  auto data = std::span(pixels, width * height * channels);
  auto image_ci = ImageCreateInformation::CreateTexture2D(width, height, Walle::Format::E_R8G8B8A8_UNORM, 1);
  image_ = Image(image_ci, SamplerSpecification());
  image_.SetImageData(std::as_bytes(data));
  io.Fonts->SetTexID((ImTextureID)&image_);
}

void ImGuiRenderer::Begin(CommandBuffer &command_buffer) {
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
  ImGui::DockSpaceOverViewport();
}

void ImGuiRenderer::End(CommandBuffer &command_buffer, const Swapchain &swapchain) {
  ImGui::Render();

  auto image_view = swapchain.GetCurrentImageView();

  std::array<Walle::RenderingAttachmentInfo, 1> color;
  color[0].imageView = image_view;
  color[0].imageLayout = Walle::ImageLayout::E_COLOR_ATTACHMENT_OPTIMAL;
  color[0].loadOp = Walle::AttachmentLoadOp::E_CLEAR;
  color[0].storeOp = AttachmentStoreOp::E_STORE;
  color[0].clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

  command_buffer.CommandBeginRendering(swapchain.GetExtent(), color);

  RenderDrawData(command_buffer);
  command_buffer.CommandEndRendering();
}

} // namespace Yuggoth