#ifndef YUGGOTH_CONSOLE_SINK_H
#define YUGGOTH_CONSOLE_SINK_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include "console_window.h"

namespace Yuggoth {

class ConsoleWindowSink : public spdlog::sinks::base_sink<std::mutex> {
public:
  ConsoleWindowSink(ConsoleWindow *console_window);

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override;
  void flush_() override;

private:
  ConsoleWindowMessage message_;
  ConsoleWindow *console_window_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_CONSOLE_SINK_H