#ifndef YUGGOTH_LIGHT_H
#define YUGGOTH_LIGHT_H

#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

struct Light {
  Vector3f color_ = Vector3f(0.0f);
  Vector3f position_ = Vector3f(0.0f);
  Vector3f direction_ = Vector3f(0.0f);
};

} // namespace Yuggoth

#endif // YUGGOTH_LIGHT_H