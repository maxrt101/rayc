#include "rayc/rayc.h"
#include "rayc/res/font8x8.h"
#include <string.h>


// TODO: Keybinds
static void move_player(rayc_t * rayc) {
  player_t * p = &rayc->player;

  // Turn left / right
  if (rayc->input.kb[SCANCODE_LEFT].held) {
    p->angle -= TURN_RATE;
    if (p->angle < 0) { p->angle += 360; }
  }
  if (rayc->input.kb[SCANCODE_RIGHT].held) {
    p->angle += TURN_RATE;
    if (p->angle > 359) { p->angle -= 360; }
  }

  const int dx = fast_sin(p->angle) * MOVE_RATE;
  const int dy = fast_cos(p->angle) * MOVE_RATE;

  // Move forward / backward
  if (rayc->input.kb[SCANCODE_W].held) {
    p->pos.x += dx;
    p->pos.y += dy;
  }

  if (rayc->input.kb[SCANCODE_S].held) {
    p->pos.x -= dx;
    p->pos.y -= dy;
  }

  // Strafe left / right
  if (rayc->input.kb[SCANCODE_A].held) {
    p->pos.x -= dy;
    p->pos.y += dx;
  }

  if (rayc->input.kb[SCANCODE_D].held) {
    p->pos.x += dy;
    p->pos.y -= dx;
  }

  // Look up / down
  if (rayc->input.kb[SCANCODE_UP].held) {
    p->look -= V_LOOK_RATE;
  }

  if (rayc->input.kb[SCANCODE_DOWN].held) {
    p->look += V_LOOK_RATE;
  }

  // Fly / height up & down
  if (rayc->input.kb[SCANCODE_SPACE].held) {
    p->pos.z += TURN_RATE;
  }
  if (rayc->input.kb[SCANCODE_LSHIFT].held) {
    p->pos.z -= TURN_RATE;
  }
}

static void process_input(rayc_t * rayc) {
  if (rayc->input.kb[SCANCODE_GRAVE].pressed) {
    rayc->console.enabled = !rayc->console.enabled;

    if (rayc->console.enabled) {
      rayc_console_enter(rayc);
    } else {
      rayc_console_exit(rayc);
    }

    return;
  }

#if WITH_EDITOR
  if (rayc->input.kb[SCANCODE_TAB].pressed) {
    rayc->editor.enabled = !rayc->editor.enabled;
  }

  if (rayc->editor.enabled) {
    rayc_process_editor_input(rayc);
  }
#endif

  if (!rayc->console.enabled
#if WITH_EDITOR
      && !rayc->editor.enabled
#endif
  ) {
    move_player(rayc);
  }
}

void rayc_init(rayc_t * rayc) {
  ASSERT(rayc, "rayc is NULL");

  memset(rayc, 0, sizeof(rayc_t));

  rayc->running = true;
  rayc->font = &font8x8;

#if WITH_EDITOR
  rayc->editor.edit_cam.zoom = 1.0f;
#endif
}

void rayc_deinit(rayc_t * rayc) {
  ASSERT(rayc, "rayc is NULL");

  rayc_clear(rayc);
}

void rayc_main(rayc_t * rayc) {
  ASSERT(rayc, "rayc is NULL");

  process_input(rayc);

#if WITH_EDITOR
  if (rayc->editor.enabled) {
    rayc_draw_editor(rayc);
  } else
#endif
  {
    rayc_draw_scene(rayc);
  }

  if (rayc->console.enabled) {
    rayc_draw_console(rayc);
  }
}

void rayc_clear(rayc_t * rayc) {
  ASSERT(rayc, "rayc is NULL");

  map_deinit(&rayc->map);

  if (rayc->map_file) {
    rayc_file_free(rayc, rayc->map_file);
    rayc->map_file = NULL;
  }

  for (int i = 0; i < MAX_FILES; ++i) {
    if (rayc->files[i].contents) {
      file_free_buffer(&rayc->files[i]);
    }
  }

  memset(rayc->textures, 0, sizeof(rayc->textures));
  memset(rayc->files, 0, sizeof(rayc->files));
}

file_t * rayc_file_alloc(rayc_t * rayc) {
  ASSERT(rayc, "rayc is NULL");

  for (int i = 0; i < MAX_FILES; ++i) {
    if (!rayc->files[i].contents) {
      return &rayc->files[i];
    }
  }

  return NULL;
}

void rayc_file_free(rayc_t * rayc, file_t * f) {
  ASSERT(rayc, "rayc is NULL");

  for (int i = 0; i < MAX_FILES; ++i) {
    if (&rayc->files[i] == f) {
      memset(&rayc->files[i], 0, sizeof(file_t));
    }
  }
}
