#ifndef YUGGOTH_LOGGER_IPP
#define YUGGOTH_LOGGER_IPP

#define YUGGOTH_LOG_CORE_INFORMATION(...) Yuggoth::Logger::Get()->GetCoreLogger()->info(__VA_ARGS__)
#define YUGGOTH_CUSTOM_LOGGER_INFORMATION(logger_name, ...) Yuggoth::Logger::Get()->GetLoggerByName(logger_name)->info(__VA_ARGS__)

#endif // YUGGOTH_LOGGER_IPP