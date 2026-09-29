#pragma once

#include "rayc/color.h"
#include "rayc/file.h"
#include <stdint.h>

typedef enum {
  TEXTURE_NONE = 0,
  TEXTURE_BMP = 1,
} texture_type_t;

typedef struct {
  texture_type_t type;
  int32_t        width;
  int32_t        height;
  uint8_t *      data;
} texture_t;

err_t texture_from_file(texture_t * t, const file_t * f);
color_t texture_get_pixel_at(texture_t * t, int x, int y);
