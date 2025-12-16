#ifndef YUGOGTH_UUID_H
#define YUGOGTH_UUID_H

#include <cstdint>

namespace Yuggoth {

class UUID {
public:
  UUID();

  UUID(uint64_t uuid);

  operator uint64_t();
  operator uint64_t() const;

  uint64_t GetValue() const;

  bool IsValid() const;

private:
  uint64_t uuid_;
};

} // namespace Yuggoth

#endif // YUGOGTH_UUID_H