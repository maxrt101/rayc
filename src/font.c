#include "rayc/font.h"
#include "rayc/util.h"

uint8_t font_get_width(const font_t * f) {
  ASSERT_RET(f, 0);

  switch (f->type) {
    case FONT_BITMAP: return f->bitmap.width;
    default:          return 0;
  }
}

uint8_t font_get_height(const font_t * f) {
  ASSERT_RET(f, 0);

  switch (f->type) {
    case FONT_BITMAP: return f->bitmap.height;
    default:          return 0;
  }
}

uint16_t font_calc_str_width(const font_t * f, const char * s) {
  int width_total = 0, width_current = 0;
  const int dx = font_get_width(f);

  while (*s) {
    width_current += dx;

    if (*s == '\n') {
      width_total = MAX(width_total, width_current);
      width_current = 0;
    }

    s++;
  }

  return MAX(width_total, width_current);
}

uint16_t font_calc_str_height(const font_t * f, const char * s) {
  const int dy = font_get_height(f);
  int height = dy;

  while (*s) {

    if (*s == '\n') {
      height += dy;
    }

    s++;
  }

  return height;
}
