#include "logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Yuggoth {

Logger *Logger::logger_instance_ = nullptr;

Logger *Logger::Get() {
  return logger_instance_;
}

constexpr spdlog::level::level_enum GetSpdLogLevel(const Logger::LogLevel &level) {
  switch (level) {
  case Logger::LogLevel::TRACE: return spdlog::level::trace;
  case Logger::LogLevel::DEBUG: return spdlog::level::debug;
  case Logger::LogLevel::INFORMATION: return spdlog::level::info;
  case Logger::LogLevel::WARNING: return spdlog::level::warn;
  case Logger::LogLevel::ERROR: return spdlog::level::err;
  case Logger::LogLevel::CRITICAL: return spdlog::level::critical;
  case Logger::LogLevel::NONE: return spdlog::level::off;
  default: std::unreachable();
  }
}

std::shared_ptr<spdlog::logger> Logger::GetCoreLogger() {
  return core_logger_;
}

Logger::Logger(LogLevel level) {
  std::array<std::shared_ptr<spdlog::sinks::sink>, 1> sinks;

  sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  core_logger_ = std::make_shared<spdlog::logger>("Yuggoth", sinks.begin(), sinks.end());

  core_logger_->set_pattern("%^[%T][%n][%l]: %v%$");

  core_logger_->set_level(GetSpdLogLevel(level));

  logger_instance_ = this;
}

void Logger::RegisterLogger(std::shared_ptr<spdlog::logger> logger) {
  spdlog::register_logger(logger);
}

std::shared_ptr<spdlog::logger> Logger::GetLoggerByName(std::string_view name) {
  return spdlog::get(name.data());
}

} // namespace Yuggoth