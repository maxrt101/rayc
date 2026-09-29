#include "rayc/color.h"

color_t color_from_rgba(const uint32_t rgba) {
  return (color_t) {
    .r = (rgba & 0xFF000000) >> 24,
    .g = (rgba & 0x00FF0000) >> 16,
    .b = (rgba & 0x0000FF00) >> 8,
    .a = (rgba & 0x000000FF)
  };
}

color_t color_from_argb(const uint32_t argb) {
  return (color_t) {
    .a = (argb & 0xFF000000) >> 24,
    .r = (argb & 0x00FF0000) >> 16,
    .g = (argb & 0x0000FF00) >> 8,
    .b = (argb & 0x000000FF)
  };
}

void color_add(color_t * c, const int v) {
  ASSERT_RET(c);

  c->r = CAP_MAX(c->r + v, 255);
  c->g = CAP_MAX(c->g + v, 255);
  c->b = CAP_MAX(c->b + v, 255);
}

void color_sub(color_t * c, const int v) {
  ASSERT_RET(c);

  c->r = CAP_MIN(c->r - v, 0);
  c->g = CAP_MIN(c->g - v, 0);
  c->b = CAP_MIN(c->b - v, 0);
}
