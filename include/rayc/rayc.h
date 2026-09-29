#pragma once

#include "rayc/defs.h"
#include "rayc/util.h"
#include "rayc/math.h"
#include "rayc/file.h"
#include "rayc/font.h"
#include "rayc/texture.h"
#include "rayc/files/map.h"
#include "rayc/input.h"
#include "rayc/render.h"
#include "rayc/console.h"

#if WITH_EDITOR
#include "rayc/editor.h"
#endif

#define TARGET_FRAME_TIME_MS (1000/FPS_LOCK)

#define HALF_WIDTH  ((WIDTH)/2)
#define HALF_HEIGHT ((HEIGHT)/2)

typedef struct {
  vec3i_t pos;
  int     angle;
  int     look;
} player_t;

typedef struct rayc_s {
  bool running;

  player_t  player;
  map_t     map;
  file_t *  map_file;
  font_t *  font;

  sector_runtime_t sector_rt[MAX_SECTORS];
  float z_buffer[WIDTH * HEIGHT];

  file_t    files[MAX_FILES];
  texture_t textures[MAX_TEXTURES];

  input_t input;

  console_t console;

#if WITH_EDITOR
  editor_t editor;
#endif
} rayc_t;

void rayc_init(rayc_t * rayc);
void rayc_deinit(rayc_t * rayc);

void rayc_main(rayc_t * rayc);

void rayc_clear(rayc_t * rayc);

file_t * rayc_file_alloc(rayc_t * rayc);
void rayc_file_free(rayc_t * rayc, file_t * f);
