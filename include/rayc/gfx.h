#pragma once

#include "rayc/color.h"
#include "rayc/font.h"

void gfx_draw_pixel(int x, int y, color_t color);
void gfx_draw_line(int x1, int y1, int x2, int y2, color_t color);
void gfx_draw_rect(int x, int y, int w, int h, color_t color);
void gfx_fill_rect(int x, int y, int w, int h, color_t color);

void gfx_draw_char(const font_t * f, int x, int y, color_t color, char c);
void gfx_draw_string(const font_t * f, int x, int y, color_t color, const char * s);