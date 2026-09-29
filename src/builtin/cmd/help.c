#include "rayc/rayc.h"

void builtin_cmd_help(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]) {
  if (argc != 2) {
    rayc_console_puts(rayc, COLOR_RED, "Usage: help CMD");
    return;
  }

  const console_cmd_t * cmd = rayc_console_get_cmd(argv[1]);

  if (cmd) {
    rayc_console_puts(rayc, COLOR_WHITE, cmd->help);
  } else {
    rayc_console_puts(rayc, COLOR_RED, "No such command");
  }
}
