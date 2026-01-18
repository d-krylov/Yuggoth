#include "terrain.h"

namespace Yuggoth {

void Terrain::SetSize(const Vector2u &size) {
  size_ = size;
}

void Terrain::SetDensity(const Vector2f &density) {
  density_ = density;
}

const Vector2u &Terrain::GetSize() const {
  return size_;
}

const Vector2f &Terrain::GetDensity() const {
  return density_;
}

void Terrain::Generate() {
}

} // namespace Yuggoth