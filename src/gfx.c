#include "rayc/gfx.h"
#include "rayc/port.h"
#include <math.h>

void gfx_draw_pixel(const int x, const int y, const color_t color) {
  port_gfx_draw_pixel(x, y, color);
}

void gfx_draw_line(const int x1, const int y1, const int x2, const int y2, const color_t color) {
  int dx = x2 - x1;
  int dy = y2 - y1;

  int slope = MAX(abs(dx), abs(dy));

  if (slope == 0) {
    gfx_draw_pixel(x1, y1, color);
    return;
  }

  float x_inc = (float)dx / slope;
  float y_inc = (float)dy / slope;

  float x = (float)x1;
  float y = (float)y1;

  for (int i = 0; i <= slope; ++i) {
    gfx_draw_pixel((int)roundf(x), (int)roundf(y), color);

    x += x_inc;
    y += y_inc;
  }
}

void gfx_draw_rect(const int x, const int y, const int w, const int h, const color_t color) {
  const int x1 = x;
  const int y1 = y;
  const int x2 = x + w;
  const int y2 = y + h;

  gfx_draw_line(x1, y1, x2, y1, color);
  gfx_draw_line(x2, y1, x2, y2, color);
  gfx_draw_line(x1, y2, x2, y2, color);
  gfx_draw_line(x1, y1, x1, y2, color);
}

void gfx_fill_rect(const int x, const int y, const int w, const int h, const color_t color) {
  for (int _y = y; _y < y + h; ++_y) {
    for (int _x = x; _x < y + w; ++_x) {
      gfx_draw_pixel(_x, _y, color);
    }
  }
}

void gfx_draw_char(const font_t * f, const int x, const int y, const color_t color, const char c) {
  ASSERT_RET(f);

  switch (f->type) {
    case FONT_BITMAP: {
      ASSERT_RET(c >= f->bitmap.codepoint.from);
      ASSERT_RET(c <= f->bitmap.codepoint.to);

      const int char_idx = c - f->bitmap.codepoint.from;
      const int bytes_per_row = (f->bitmap.width + 7) / 8;

      for (int row = 0; row < f->bitmap.height; ++row) {
        const int data_offset = (char_idx * f->bitmap.height + row) * bytes_per_row;

        for (int col = 0; col < f->bitmap.width; ++col) {
          const int byte_index = col / 8;
          const int bit_index = 7 - (col % 8);

          const uint8_t pixel_data = f->bitmap.data[data_offset + byte_index];

          if (pixel_data & (1 << (7 - bit_index))) {
            gfx_draw_pixel(x + col, y + row, color);
          }
        }
      }
      break;
    }
    default:
      break;
  }
}

void gfx_draw_string(const font_t * f, const int x, const int y, const color_t color, const char * s) {
  const int dx = font_get_width(f);
  const int dy = font_get_height(f);

  int tx = x;
  int ty = y;

  while (*s) {
    gfx_draw_char(f, tx, ty, color, *s);

    tx += dx;
    if (*s == '\n') { ty += dy; tx = x; }
    s++;
  }
}
