#ifndef YUGOGTH_UUID_H
#define YUGOGTH_UUID_H

#include <cstdint>

namespace Yuggoth {

class UUID {
public:
  UUID();

private:
  uint64_t uuid_;
};

} // namespace Yuggoth

#endif // YUGOGTH_UUID_H