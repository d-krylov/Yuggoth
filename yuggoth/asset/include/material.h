#ifndef YUGGOTH_MATERIAL_H
#define YUGGOTH_MATERIAL_H

#include "asset.h"
#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

struct Material {
  int32_t color_texture_index_ = -1;
  int32_t normal_texture_index_ = -1;
  int32_t emissive_texture_index_ = -1;
  Vector4f base_color_ = Vector4f(1.0f);
  float metallic_ = 1.0f;
  float roughness_ = 1.0f;
};

} // namespace Yuggoth

#endif // YUGGOTH_MATERIAL_H