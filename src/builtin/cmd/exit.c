#include "rayc/rayc.h"

void builtin_cmd_exit(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]) {
  rayc->running = false;
}
