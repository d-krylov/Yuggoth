#ifndef YUGGOTH_TRANSFORM_H
#define YUGGOTH_TRANSFORM_H

#include "mathematics_types.h"

namespace Yuggoth {

class Transform {
public:
  Transform(const Vector3f &translation = Vector3f(0.0f), const Vector3f &scale = Vector3f(1.0f));

  void SetTranslation(const Vector3f &translation);
  void SetOrientation(const Vector3f &orientation);
  void SetScale(const Vector3f &scale);

  const Vector3f &GetTranslation() const;
  const Vector3f &GetRotation() const;
  const Vector3f &GetScale() const;

  Matrix4f GetMatrix() const;

private:
  Vector3f translation_;
  Vector3f scale_;
  Vector3f euler_;
  Quaternion orientation_;
};

} // namespace Yuggoth

#endif // YUGGOTH_TRANSFORM_H