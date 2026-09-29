#include "rayc/files/map.h"
#include "rayc/util.h"
#include <string.h>

// FIXME: Do better
#define HEADER_SIZE (                 \
  /* magic */ 4                     + \
  sizeof_field(map_t, start.x)      + \
  sizeof_field(map_t, start.y)      + \
  sizeof_field(map_t, start.z)      + \
  sizeof_field(map_t, sector_count) + \
  sizeof_field(map_t, wall_count))

err_t map_from_file(map_t * m, file_t * f) {
  ASSERT_RET(m && f, E_NULL);

  if (file_read_u4(f) != MAP_MAGIC) {
    return E_FORMAT;
  }

  m->start.x = file_read_i2(f);
  m->start.y = file_read_i2(f);
  m->start.z = file_read_i2(f);
  m->sector_count = file_read_u2(f);
  m->wall_count = file_read_u2(f);

  // m->sectors = (sector_t *)(f->contents + HEADER_SIZE);
  // m->walls = (wall_t *)(f->contents + HEADER_SIZE + m->sector_count * sizeof(sector_t));

  sector_t * sectors = (sector_t *)(f->contents + HEADER_SIZE);
  wall_t * walls = (wall_t *)(f->contents + HEADER_SIZE + m->sector_count * sizeof(sector_t));

#if WITH_EDITOR
  m->sector_capacity = m->sector_count + 16;
  m->sectors = malloc(m->sector_capacity * sizeof(sector_t));
  memcpy(m->sectors, sectors, m->sector_count * sizeof(sector_t));

  m->wall_capacity = m->wall_count + 64;
  m->walls = malloc(m->wall_capacity * sizeof(wall_t));
  memcpy(m->walls, walls, m->wall_count * sizeof(wall_t));
#else
  m->sectors = sectors;
  m->walls = walls;
#endif

  return E_OK;
}

err_t map_to_file(const map_t * m, void * f) {
  ASSERT_RET(m && f, E_NULL);

  file_write_u4(f, MAP_MAGIC);
  file_write_i2(f, m->start.x);
  file_write_i2(f, m->start.y);
  file_write_i2(f, m->start.z);
  file_write_u2(f, m->sector_count);
  file_write_u2(f, m->wall_count);

  file_write(f, m->sector_count * sizeof(sector_t), (void*) m->sectors);
  file_write(f, m->wall_count * sizeof(wall_t), (void*) m->walls);

  return E_OK;
}

void map_deinit(map_t * m) {
  ASSERT_RET(m);

#if WITH_EDITOR
  if (m->sectors) { free(m->sectors); }
  if (m->walls)   { free(m->walls);   }
#endif

  memset(m, 0, sizeof(map_t));
}

size_t map_size(map_t * m) {
  return HEADER_SIZE + m->sector_count * sizeof(sector_t) + m->wall_count * sizeof(wall_t);
}

#if WITH_EDITOR
void map_insert_wall(map_t * m, int sector_idx, int local_wall_idx, wall_t new_wall) {
  if (m->wall_count >= m->wall_capacity) {
    m->wall_capacity = (m->wall_capacity == 0) ? 64 : m->wall_capacity * 2;
    m->walls = realloc(m->walls, m->wall_capacity * sizeof(wall_t));
  }

  sector_t * sec = &m->sectors[sector_idx];

  // local_wall_idx is where inside the sector we want it (0 to sec->we - sec->ws)
  int global_wall_idx = sec->ws + local_wall_idx;

  // Shift all subsequent walls one slot to the right
  if (global_wall_idx < m->wall_count) {
    memmove(&m->walls[global_wall_idx + 1],
            &m->walls[global_wall_idx],
            (m->wall_count - global_wall_idx) * sizeof(wall_t));
  }

  // Insert the new wall
  m->walls[global_wall_idx] = new_wall;
  m->wall_count++;

  // Update the current sector's end index
  sec->we++;

  // Shift the indices of all sectors that come after this one
  for (int i = sector_idx + 1; i < m->sector_count; i++) {
    m->sectors[i].ws++;
    m->sectors[i].we++;
  }
}

int map_add_sector(map_t * m, int16_t z_floor, int16_t z_ceil) {
  if (m->sector_count >= m->sector_capacity) {
    m->sector_capacity = (m->sector_capacity == 0) ? 16 : m->sector_capacity * 2;
    m->sectors = realloc(m->sectors, m->sector_capacity * sizeof(sector_t));
  }

  int new_idx = m->sector_count;
  sector_t * sec = &m->sectors[new_idx];

  // Point the new sector's wall indices to the current end of the wall array
  sec->ws = m->wall_count;
  sec->we = m->wall_count; // Empty initially
  sec->z1 = z_floor;
  sec->z2 = z_ceil;
  sec->tf = 0; // Default textures
  sec->tc = 0;

  m->sector_count++;
  return new_idx;
}

void map_delete_sector(map_t * m, int sector_idx) {
  if (sector_idx < 0 || sector_idx >= m->sector_count) return;

  sector_t * sec = &m->sectors[sector_idx];

  int ws = sec->ws;
  int we = sec->we;
  int num_walls = we - ws;

  // Shift the wall array left to overwrite the deleted sector's walls
  if (we < m->wall_count) {
    memmove(&m->walls[ws], &m->walls[we],
            (m->wall_count - we) * sizeof(wall_t));
  }
  m->wall_count -= num_walls;

  // Fix ws/we indices for all sectors that come AFTER the deleted one
  for (int i = sector_idx + 1; i < m->sector_count; i++) {
    m->sectors[i].ws -= num_walls;
    m->sectors[i].we -= num_walls;
  }

  // Shift the sector array left to overwrite the deleted sector
  if (sector_idx + 1 < m->sector_count) {
    memmove(&m->sectors[sector_idx], &m->sectors[sector_idx + 1],
            (m->sector_count - sector_idx - 1) * sizeof(sector_t));
  }
  m->sector_count--;
}

#endif
