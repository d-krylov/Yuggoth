#ifndef YUGGOTH_TRANSFORM_H
#define YUGGOTH_TRANSFORM_H

#include "mathematics_types.h"

namespace Yuggoth {

class Transform {
public:
  Transform();

private:
  Vector3f translation_;
  Vector3f scale_;
};

} // namespace Yuggoth

#endif // YUGGOTH_TRANSFORM_H