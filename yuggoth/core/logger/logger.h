#ifndef YUGGOTH_LOGGER_H
#define YUGGOTH_LOGGER_H

#include <memory>

namespace spdlog {
class logger;
};

namespace Yuggoth {

class Logger {
public:
  enum class LogLevel {
    TRACE,
    DEBUG,
    INFORMATION,
    WARNING,
    ERROR,
    CRITICAL,
    NONE
  };

  Logger(LogLevel level);

  static void RegisterLogger(std::shared_ptr<spdlog::logger> logger);

  static Logger *Get();

  std::shared_ptr<spdlog::logger> GetCoreLogger();

  std::shared_ptr<spdlog::logger> GetLoggerByName(std::string_view name);

private:
  std::shared_ptr<spdlog::logger> core_logger_;

  static Logger *logger_instance_;
};

} // namespace Yuggoth

#include "logger.ipp"

#endif // YUGGOTH_LOGGER_H