#ifndef YUGGOTH_EVENT_H
#define YUGGOTH_EVENT_H

#include "event_kind.h"

namespace Yuggoth {

class Event {
public:
  virtual ~Event() = default;

  virtual EventKind GetEventKind() = 0;

public:
  bool handled{false};
};

} // namespace Yuggoth

#endif // YUGGOTH_EVENT_H