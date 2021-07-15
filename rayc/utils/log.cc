#include "utils/log.h"
#include "utils/debug.h"

#include <cstdio>
#include <cstdarg>

#include <SDL2/SDL.h>

void rayc::sdlError(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[\033[1;31m ERROR \033[0m] ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "(%s)\n", SDL_GetError());
  va_end(args);
}

void rayc::error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[\033[1;31m ERROR \033[0m] ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}

void rayc::warning(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[\033[1;33m WARN \033[0m] ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}

void rayc::info(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stdout, "[ INFO ] ");
  vfprintf(stdout, fmt, args);
  fprintf(stdout, "\n");
  va_end(args);
}

void rayc::debug(const char* fmt, ...) {
  if (isDebugEnabled()) {
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[\033[1;34m DEBUG \033[0m] ");
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
  }
}