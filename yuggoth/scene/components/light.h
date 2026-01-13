#ifndef YUGGOTH_LIGHT_H
#define YUGGOTH_LIGHT_H

#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/core/tools/core.h"

namespace Yuggoth {

struct Light {
  static inline const std::size_t type_id = GetTypeId<Light>();

  Vector3f color_ = Vector3f(0.0f);
  Vector3f position_ = Vector3f(0.0f);
  Vector3f direction_ = Vector3f(0.0f);
  Vector3f attenuation = Vector3f(0.0, 0.0, 1.0);
};

} // namespace Yuggoth

#endif // YUGGOTH_LIGHT_H