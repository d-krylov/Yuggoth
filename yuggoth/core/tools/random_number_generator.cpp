#include "yuggoth/core/tools/include/random_number_generator.h"
#include <random>

namespace Yuggoth {

namespace {

std::random_device random_device;
std::mt19937_64 engine(random_device());
std::uniform_int_distribution<uint64_t> uniform_distribution;

} // namespace

uint64_t RandomNumberGenerator::UniformU64() {
  return uniform_distribution(engine);
}

} // namespace Yuggoth