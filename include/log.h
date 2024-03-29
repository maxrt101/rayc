#ifndef _RAYC_LOG_H_
#define _RAYC_LOG_H_ 1

#include <string>
#include <cstdarg>

namespace rayc {

enum class LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

LogLevel stringToLogLevel(const std::string& str);
std::string logLevelToString(LogLevel level);

void debug(const std::string format, ...);
void info(const std::string format, ...);
void warning(const std::string format, ...);
void error(const std::string format, ...);
void fatal(const std::string format, ...);
void sdlError(std::string format, ...);
void sdlFatal(std::string format, ...);

void logf(LogLevel level, const std::string format, ...);
void vlogf(LogLevel level, const std::string& format, va_list args);
void vflogf(FILE* dest, LogLevel level, const std::string& format, va_list args);

LogLevel getLogLevel();
void setLogLevel(LogLevel level);

} /* namespace rayc */

#endif /* _RAYC_LOG_H_ */