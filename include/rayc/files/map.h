#pragma once

#include "rayc/err.h"
#include "rayc/file.h"
#include <stdint.h>

#define MAP_MAGIC 0x524d4150 // b"RMAP"

typedef struct {
  int16_t x1, y1;
  int16_t x2, y2;
  uint8_t texture;
  int8_t  u, v;
} wall_t;

typedef struct {
  uint16_t ws, we;
  int16_t  z1, z2;
  uint8_t  tf, tc;
} sector_t;

typedef struct {
  struct {
    int16_t x, y, z;
  } start;

  uint16_t sector_count;
  uint16_t wall_count;

#if WITH_EDITOR
  uint16_t sector_capacity;
  uint16_t wall_capacity;
#endif

  sector_t * sectors;
  wall_t *   walls;
} map_t;

err_t map_from_file(map_t * m, file_t * f);
err_t map_to_file(const map_t * m, void * f);

void map_deinit(map_t * m);

size_t map_size(map_t * m);

#if WITH_EDITOR
void map_insert_wall(map_t * m, int sector_idx, int local_wall_idx, wall_t new_wall);
int map_add_sector(map_t * m, int16_t z_floor, int16_t z_ceil);
void map_delete_sector(map_t * m, int sector_idx);
#endif

// size_t map_add_wall(map_t * m, wall_t * w);
// size_t map_add_sector(map_t * m, sector_t * s);
