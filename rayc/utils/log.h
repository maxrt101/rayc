#ifndef _RAYC_UTILS_LOG_H
#define _RAYC_UTILS_LOG_H

namespace rayc {

void sdlError(const char* fmt, ...);
void error(const char* fmt, ...);
void warning(const char* fmt, ...);
void info(const char* fmt, ...);
void debug(const char* fmt, ...);

} /* namespace rayc */

#endif