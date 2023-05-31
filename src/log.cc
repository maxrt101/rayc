#include <rayc/log.h>

#include <cctype>
#include <cstdio>
#include <algorithm>
#include <SDL2/SDL.h>

#define _VLOGF_INTERNAL_F(level, format) \
  va_list args; \
  va_start(args, format); \
  vlogf(level, format, args); \
  va_end(args);

#define _VLOGF_INTERNAL(level) _VLOGF_INTERNAL_F(level, format)


static rayc::LogLevel g_logLevel = rayc::LogLevel::WARNING;


rayc::LogLevel rayc::stringToLogLevel(const std::string& s) {
  std::string str = s;
  std::transform(str.begin(), str.end(), str.begin(), tolower);
  if (str == "debug") {
    return LogLevel::DEBUG;
  } else if (str == "info") {
    return LogLevel::INFO;
  } else if (str == "warning") {
    return LogLevel::WARNING;
  } else if (str == "error") {
    return LogLevel::ERROR;
  } else if (str == "fatal") {
    return LogLevel::FATAL;
  }
  return LogLevel::DEBUG;
}

std::string rayc::logLevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::DEBUG:
      return "debug";
    case LogLevel::INFO:
      return "info";
    case LogLevel::WARNING:
      return "warning";
    case LogLevel::ERROR:
      return "error";
    case LogLevel::FATAL:
      return "fatal";
    default:
      return "?";
  }
}

rayc::LogLevel rayc::getLogLevel() {
  return g_logLevel;
}

void rayc::setLogLevel(LogLevel level) {
  g_logLevel = level;
}

void rayc::debug(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::DEBUG);
}

void rayc::info(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::INFO);
}

void rayc::warning(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::WARNING);
}

void rayc::error(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::ERROR);
}

void rayc::fatal(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::FATAL);
}

void rayc::sdlError(std::string format, ...) {
  format += " (";
  format += SDL_GetError();
  format += ")";
  _VLOGF_INTERNAL(LogLevel::ERROR);
}

void rayc::sdlFatal(std::string format, ...) {
  format += " (";
  format += SDL_GetError();
  format += ")";
  _VLOGF_INTERNAL(LogLevel::FATAL);
}

void rayc::logf(LogLevel level, const std::string format, ...) {
  va_list args;
  va_start(args, format);
  vlogf(level, format, args);
  va_end(args);
}

void rayc::vlogf(LogLevel level, const std::string& format, va_list args) {
  if (level < g_logLevel) return;

  FILE* dest = stdout;
  if (level > LogLevel::INFO) {
    dest = stderr;
  }

  vflogf(dest, level, format, args);
}

void rayc::vflogf(FILE* dest, LogLevel level, const std::string& format, va_list args) {
  switch (level) {
    case LogLevel::FATAL:   fprintf(dest, "\033[41mFATAL");   break;
    case LogLevel::ERROR:   fprintf(dest, "\033[31mERROR");   break;
    case LogLevel::WARNING: fprintf(dest, "\033[33mWARNING"); break;
    case LogLevel::INFO:    fprintf(dest, "\033[36mINFO");    break;
    case LogLevel::DEBUG:   fprintf(dest, "\033[34mDEBUG");   break;
    default:                fprintf(dest, "<?>");             break;
  }

  fprintf(dest, "\033[0m: ");
  vfprintf(dest, format.c_str(), args);
  fprintf(dest, "\n");
}
