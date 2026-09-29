#pragma once

#include "rayc/util.h"
#include <stdint.h>

#define RGBA(__r, __g, __b, __a) ((color_t){ .r = __r, .g = __g, .b = __b, .a = __a })
#define RGB(__r, __g, __b) RGBA(__r, __g, __b, 255)


#define COLOR_BLACK   RGB(0,   0,   0  )
#define COLOR_WHITE   RGB(255, 255, 255)
#define COLOR_RED     RGB(255, 0,   0  )
#define COLOR_GREEN   RGB(0,   255, 0  )
#define COLOR_BLUE    RGB(0,   0,   255)
#define COLOR_YELLOW  RGB(255, 255, 0  )
#define COLOR_MAGENTA RGB(255, 0,   255)
#define COLOR_CYAN    RGB(0,   255, 255)

#define COLOR_TRANSPARENT RGBA(0, 0, 0, 0)

typedef struct __PACKED {
  uint8_t r, g, b, a;
} color_t;

color_t color_from_rgba(uint32_t rgba);
color_t color_from_argb(uint32_t argb);

void color_add(color_t * c, int v);
void color_sub(color_t * c, int v);
