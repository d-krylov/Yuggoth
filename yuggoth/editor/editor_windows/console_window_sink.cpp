#include "console_window_sink.h"

namespace Yuggoth {

ConsoleWindowSink::ConsoleWindowSink(ConsoleWindow *console_window) : console_window_(console_window) {
}

void ConsoleWindowSink::sink_it_(const spdlog::details::log_msg &message) {
  spdlog::memory_buf_t formatted;
  spdlog::sinks::base_sink<std::mutex>::formatter_->format(message, formatted);

  message_.message_ = fmt::to_string(formatted);
  flush_();
}

void ConsoleWindowSink::flush_() {
  console_window_->PushMessage(message_);
}

} // namespace Yuggoth