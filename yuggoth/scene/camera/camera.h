#ifndef YUGGOTH_CAMERA_H
#define YUGGOTH_CAMERA_H

#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

class Camera {
public:
  Camera();

  void SetPosition(const Vector3f &position);
  void SetAspect(float aspect);
  void SetFOV(float fov);

  const Vector3f &GetPosition() const;
  float GetAspect() const;
  float GetFOV() const;

private:
  Vector3f position_;
  Quaternion orientation_;
  float aspect_;
  float near_;
  float far_;
  float fov_;
};

} // namespace Yuggoth

#endif // YUGGOTH_CAMERA_H