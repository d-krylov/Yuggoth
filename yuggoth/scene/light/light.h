#ifndef YUGGOTH_LIGHT_H
#define YUGGOTH_LIGHT_H

#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

struct Light {
  Vector3f color_;
  Vector3f position_;
};

} // namespace Yuggoth

#endif // YUGGOTH_LIGHT_H