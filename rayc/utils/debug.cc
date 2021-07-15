#include "utils/debug.h"

#ifdef DEBUG
static bool debug_flag = true;
#else
static bool debug_flag = false;
#endif

void rayc::setDebug(bool value) {
  debug_flag = value;
}

bool rayc::isDebugEnabled() {
  return debug_flag;
}