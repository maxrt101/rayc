#include "rayc/rayc.h"
#include "rayc/files/resmap.h"
#include <string.h>

void builtin_cmd_resmap(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]) {
  if (argc > 1 && !strcmp(argv[1], "load")) {
    if (argc != 3) {
      rayc_console_puts(rayc, COLOR_RED, "Usage: resmap load FILE");
      return;
    }

    file_t resmap;
    if (file_read_to_buffer(&resmap, argv[2])) { die("Failed to open test %s", argv[2]); }

    resmap_load(rayc, &resmap);

    rayc_console_printf(rayc, COLOR_WHITE, "Loaded %s", argv[2]);
    return;
  }

  rayc_console_puts(rayc, COLOR_RED, "Usage: resmap save|load|add|dump");
}
