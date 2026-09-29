#include "rayc/rayc.h"

void builtin_cmd_test(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]) {
  for (int i = 0; i < argc; ++i) {
    rayc_console_puts(rayc, COLOR_WHITE, argv[i]);
  }
}
