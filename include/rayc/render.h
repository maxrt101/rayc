#pragma once

#include "rayc/defs.h"
#include <stdint.h>

typedef struct rayc_s rayc_t;

typedef struct {
  uint16_t map_sector_idx;
  int16_t  distance;
  int8_t   surface;
  int16_t  surf[WIDTH];
} sector_runtime_t;

void rayc_draw_scene(rayc_t * rayc);

