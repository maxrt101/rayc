#pragma once

#include <stdint.h>

typedef enum {
  FONT_NONE = 0,
  FONT_BITMAP
} font_type_t;

typedef struct {
  font_type_t type;
  union {
    struct {
      uint8_t * data;
      uint8_t   width;
      uint8_t   height;
      struct {
        uint8_t from, to;
      } codepoint;
    } bitmap;
  };
} font_t;

uint8_t font_get_width(const font_t * f);
uint8_t font_get_height(const font_t * f);

uint16_t font_calc_str_width(const font_t * f, const char * s);
uint16_t font_calc_str_height(const font_t * f, const char * s);
