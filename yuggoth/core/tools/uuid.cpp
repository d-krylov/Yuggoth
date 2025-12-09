#include "yuggoth/core/tools/uuid.h"
#include "yuggoth/core/tools/random_number_generator.h"

namespace Yuggoth {

UUID::UUID() {
  uuid_ = RandomNumberGenerator::UniformU64();
}

UUID::UUID(uint64_t uuid) : uuid_(uuid) {
}

UUID::operator uint64_t() {
  return uuid_;
}

UUID::operator uint64_t() const {
  return uuid_;
}

uint64_t UUID::GetValue() const {
  return uuid_;
}

bool UUID::IsValid() const {
  return uuid_ != 0;
}

} // namespace Yuggoth