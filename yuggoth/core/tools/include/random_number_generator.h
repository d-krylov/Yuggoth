#ifndef YUGGOTH_RANDOM_NUMBER_GENERATOR_H
#define YUGGOTH_RANDOM_NUMBER_GENERATOR_H

#include <cstdint>

namespace Yuggoth {

class RandomNumberGenerator {
public:
  static uint64_t UniformU64();
};

} // namespace Yuggoth

#endif // YUGGOTH_RANDOM_NUMBER_GENERATOR_H