#pragma once

#include "rayc/util.h"
#include "rayc/color.h"
#include "../file.h"
#include <stdint.h>

#define BMP_MAGIC 0x4D42

typedef struct __PACKED {
  uint16_t type;        // Magic identifier: "BM"
  uint32_t size;        // File size in bytes
  uint16_t __reserved1;
  uint16_t __reserved2;
  uint32_t offset_bits; // Offset to image data pixels
} bmp_file_header_t;

typedef struct __PACKED {
  uint32_t size;      // Header size (40 bytes)
  int32_t  width;     // Width in pixels
  int32_t  height;    // Height in pixels (positive = bottom-up, negative = top-down)
  int32_t  planes;    // Number of color planes (must be 1)
  int32_t  bit_count; // Bits per pixel (e.g., 24 for RGB)
  int32_t  compression;
  int32_t  size_image;
  int32_t  x_pels_per_meter;
  int32_t  y_pels_per_meter;
  int32_t  clr_used;
  int32_t  clr_important;
} bmp_info_header_t;

bool bmp_probe(const file_t * f);
uint8_t * bmp_get_image_data(const file_t * f);
err_t bmp_get_dimensions(const file_t * f, int32_t * w, int32_t * h);
color_t bmp_get_pixel_at(const uint8_t * data, int32_t h, int x, int y);
