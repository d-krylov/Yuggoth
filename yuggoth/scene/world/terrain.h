#ifndef YUGGOTH_TERRAIN_H
#define YUGGOTH_TERRAIN_H

#include <yuggoth/mathematics/include/mathematics_types.h>
#include <yuggoth/asset/core/renderable.h>
#include <vector>

namespace Yuggoth {

class Terrain : public Renderable {
public:
  void Generate();

  void SetSize(const Vector2u &size);
  void SetDensity(const Vector2f &density);

  const Vector2u &GetSize() const;
  const Vector2f &GetDensity() const;

private:
  Vector2u size_ = Vector2u(0, 0);
  Vector2f density_ = Vector2f(1.0f, 1.0f);

  std::vector<float> height;
};

} // namespace Yuggoth

#endif // YUGGOTH_TERRAIN_H