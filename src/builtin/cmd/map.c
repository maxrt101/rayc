#include "rayc/rayc.h"
#include <string.h>
#include <stdio.h>

void builtin_cmd_map(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]) {
  const map_t * m = &rayc->map;

  if (argc > 1 && !strcmp(argv[1], "save")) {
    if (argc != 3) {
      rayc_console_puts(rayc, COLOR_RED, "Usage: map save FILE");
      return;
    }

    void * f = file_open_for_writing(argv[2]);
    map_to_file(m, f);
    file_close_for_writing(f);

    rayc_console_printf(rayc, COLOR_WHITE, "Saved to %s", argv[2]);
    return;
  }

  if (argc > 1 && !strcmp(argv[1], "load")) {
    if (argc != 3) {
      rayc_console_puts(rayc, COLOR_RED, "Usage: map load FILE");
      return;
    }

    map_deinit(&rayc->map);

    if (rayc->map_file) {
      rayc_file_free(rayc, rayc->map_file);
    }

    rayc->map_file = rayc_file_alloc(rayc);
    if (file_read_to_buffer(rayc->map_file, argv[2])) { die("Failed to open %s", argv[2]); }
    if (map_from_file(&rayc->map, rayc->map_file))    { die("Failed to load %s", argv[2]); }

    rayc->player.pos.x = rayc->map.start.x;
    rayc->player.pos.y = rayc->map.start.y;
    rayc->player.pos.z = rayc->map.start.z;

    rayc_console_printf(rayc, COLOR_WHITE, "Loaded %s", argv[2]);
    return;
  }

  if (argc > 1 && !strcmp(argv[1], "unload")) {
    map_deinit(&rayc->map);

    if (rayc->map_file) {
      rayc_file_free(rayc, rayc->map_file);
      rayc->map_file = NULL;
    }

    return;
  }

  if (argc > 1 && !strcmp(argv[1], "dump")) {
    printf(
  "MAP\n"
  "Player x=%d y=%d z=%d\n"
  "Sectors:\n",
  m->start.x,
  m->start.y,
  m->start.z
);

    for (int s = 0; s < m->sector_count; ++s) {
      printf(
        "  Sector #%d:\n"
        "    ws=%d we=%d\n"
        "    z1=%d z2=%d\n"
        "    tf=%d tc=%d\n",
        s,
        m->sectors[s].ws, m->sectors[s].we,
        m->sectors[s].z1, m->sectors[s].z2,
        m->sectors[s].tf, m->sectors[s].tc
      );

      for (int w = m->sectors[s].ws; w < m->sectors[s].we; ++w) {
        printf(
          "    Wall #%d:\n"
          "      x1=%d y1=%d\n"
          "      x2=%d y2=%d\n"
          "      texture=%d\n"
          "      u=%d v=%d\n",
          w,
          m->walls[w].x1, m->walls[w].y1,
          m->walls[w].x2, m->walls[w].y2,
          m->walls[w].texture,
          m->walls[w].u, m->walls[w].v
        );
      }
    }

    rayc_console_puts(rayc, COLOR_WHITE, "Dumped (stdout)");
    return;
  }

  rayc_console_puts(rayc, COLOR_RED, "Usage: map save|load|dump");
}
