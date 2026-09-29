#include "rayc/res/icons8x8.h"

static char icons8x8_basic_data[128][8] = {
  [ICONS8X8_CURSOR] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00000000,
  },
};

font_t icons8x8 = {
  .type = FONT_BITMAP,
  .bitmap = {
    .data = (uint8_t*)icons8x8_basic_data,
    .width = 8,
    .height = 8,
    .codepoint = {
      .from = 0,
      .to = 0x7f
    }
  }
};
