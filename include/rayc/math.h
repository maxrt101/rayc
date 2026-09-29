#pragma once

#include <math.h>

#define RAD_TO_DEG(__rad) ((__rad) * (180.0f / M_PI))
#define DEG_TO_RAD(__deg) ((__deg) * (M_PI / 180.0f))

typedef struct { int x, y, z; } vec3i_t;

float fast_sin(int a);
float fast_cos(int a);

int distance(int x1, int y1, int x2, int y2);
