#ifndef YUGGOTH_CAMERA_H
#define YUGGOTH_CAMERA_H

#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

class Camera {
public:
  Camera(const Vector3f &position = Vector3f(0.0, 0.0f, 0.0f), const Vector3f &orientation = Vector3f(0.0, 0.0f, 0.0f));

  void SetPosition(const Vector3f &position);
  void SetOrientation(const Vector3f &orientation);
  void SetAspect(float aspect);
  void SetFOV(float fov);
  void SetFar(float far);

  Matrix4f GetLookAt() const;
  Matrix4f GetProjection() const;

  Vector3f GetEulerAngles() const;

  const Vector3f &GetPosition() const;
  float GetAspect() const;
  float GetFOV() const;
  float GetFar() const;

protected:
  void UpdateCameraVectors();

private:
  Vector3f position_;
  Vector3f front_;
  Vector3f right_;
  Vector3f up_ = Vector3f(0.0, 1.0f, 0.0f);
  Quaternion orientation_;
  Vector3f euler_;
  float aspect_ = 1.0f;
  float near_ = 0.1f;
  float far_ = 1000.0f;
  float fov_{1.0f};
};

} // namespace Yuggoth

#endif // YUGGOTH_CAMERA_H