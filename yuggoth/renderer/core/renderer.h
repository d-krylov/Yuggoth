#ifndef YUGGOTH_RENDERER_H
#define YUGGOTH_RENDERER_H

#include "renderer_context.h"
#include "base_renderer_backend.h"
#include "yuggoth/graphics/descriptors/descriptor_pool.h"
#include "yuggoth/graphics/descriptors/descriptor_set.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/image/image2D.h"
#include "yuggoth/graphics/image/image_depth.h"
#include "yuggoth/graphics/acceleration/acceleration_structure.h"

namespace Yuggoth {

class Renderer {
public:
  Renderer(const RendererContext &renderer_context);

  const RendererContext &GetRendererConstext() const;

  const Image2D &GetImage() const;

  void OnViewportResize(uint32_t width, uint32_t height);

  void Begin(Scene *scene);

  void Draw(Scene *scene);

  void End();

  const AccelerationStructure &GetTopAccelerationStructure() const;

protected:
  void Create();
  void CreateDescriptorSets();
  void BuildBottomAccelerationStructures(Scene *scene);
  void BuildTopAccelerationStructure();

private:
  RendererContext renderer_context_;
  DescriptorPool descriptor_pool_;
  DescriptorSet descriptor_set_;
  CommandBuffer command_buffer_;
  Image2D target_image_;
  ImageDepth depth_image_;
  BaseRendererBackend base_renderer_backend_;
  std::unordered_map<UUID, AccelerationStructure> bottom_acceleration_structures_;
  std::unordered_map<UUID, std::vector<InstanceData>> bottom_transforms_;
  AccelerationStructure top_acceleration_structure_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_H