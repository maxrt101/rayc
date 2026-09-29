#include "rayc/math.h"
#include "rayc/util.h"

static float fast_sin_table[360];
static float fast_cos_table[360];

__CONSTRUCTOR(255)
static void init_math() {
  for (int x = 0; x < 360; ++x) {
    fast_sin_table[x] = sinf((float)x / 180.0f * M_PI);
    fast_cos_table[x] = cosf((float)x / 180.0f * M_PI);
  }
}

float fast_sin(int a) {
  return fast_sin_table[a];
}

float fast_cos(int a) {
  return fast_cos_table[a];
}

int distance(int x1, int y1, int x2, int y2) {
  return (int)sqrtf((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
