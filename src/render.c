#include "rayc/rayc.h"
#include "rayc/gfx.h"

#include <stdio.h>

static void clip_behind_camera(
  int * x1, int * y1, int * z1, float * u1,
  int   x2, int   y2, int   z2, float   u2
) {
  float da = *y1;
  float db = y2;
  float d = da - db;
  if (d == 0) { d = 1; }
  float s = da / d;

  *x1 = *x1 + s * (x2 - *x1);
  *y1 = *y1 + s * (y2 - *y1);
  *z1 = *z1 + s * (z2 - *z1);

  if (u1) {
    *u1 = *u1 + s * (u2 - *u1);
  }

  if (*y1 == 0) { *y1 = 1; }
}

static int calculate_shade(const wall_t * wall) {
  float angle = RAD_TO_DEG(atan2f(wall->y2 - wall->y1, wall->x2 - wall->x1));

  if (angle < 0) {
    angle += 360;
  }

  int shade = (int)angle;

  if (shade > 180) {
    shade = 180 - (shade - 180);
  }

  if (shade > 90) {
    shade = 90 - (shade - 90);
  }

  return shade;
}

static void draw_floor_part(
  rayc_t *  rayc,
  const int x,
  const int s,
  int       y_start,
  int       y_end,
  const int wt
) {
  const player_t * p = &rayc->player;
  const map_t * m = &rayc->map;

  const sector_t * map_sec = &m->sectors[s];

  sector_runtime_t * rt_sec = &rayc->sector_rt[s];

  const int surf_mode = rt_sec->surface;

  const int floor_x = x - HALF_WIDTH;
  int wall_ofs = 0;
  int pt = wt;

  if (surf_mode == 1) {
    y_start = rt_sec->surf[x];
    wall_ofs = map_sec->z1;
    pt = map_sec->tf;
  }

  if (surf_mode == 2) {
    y_end = rt_sec->surf[x];
    wall_ofs = map_sec->z2;
    pt = map_sec->tc;
  }

  const float look_offset = (float)p->look * FOV_MODIFIER / H_LOOK_SCALE;
  const float cam_z = (float)p->pos.z - (float)wall_ofs;

  const float pa_cos = fast_cos(p->angle);
  const float pa_sin = fast_sin(p->angle);

  const int y_floor_start = y_start - HALF_HEIGHT;
  const int y_floor_end = y_end - HALF_HEIGHT;

  const int tex_w = rayc->textures[pt].width;
  const int tex_h = rayc->textures[pt].height;

  for (int y = y_floor_start; y < y_floor_end; ++y) {
    // Vertical distance from the horizon
    float z = (float)y + look_offset;
    if (z == 0.0f) z = 0.001f;

    // Un-project screen coordinates to camera-space depths/widths
    const float wy = cam_z * FOV_MODIFIER / z;
    const float wx = (float)floor_x * cam_z / z;

    // The depth of the floor pixel is exactly wy
    const float floor_depth = wy;
    const int screen_y = y + HALF_HEIGHT;

    if (screen_y >= 0 && screen_y < HEIGHT) {
      int buf_idx = x + screen_y * WIDTH;

      if (floor_depth < rayc->z_buffer[buf_idx]) {
        rayc->z_buffer[buf_idx] = floor_depth;

        // Exact inverse 2D rotation matrix to get absolute world coordinates
        const float world_x = (float)p->pos.x + (wx * pa_cos) + (wy * pa_sin);
        const float world_y = (float)p->pos.y + (wy * pa_cos) - (wx * pa_sin);

        // Map world coordinates to the texture block size
        float tx_float = world_x / MAP_GRID_SIZE;
        float ty_float = world_y / MAP_GRID_SIZE;

        // Use floorf to wrap negative coordinates in all quadrants
        tx_float -= floorf(tx_float);
        ty_float -= floorf(ty_float);

        const int tx = (int)(tx_float * tex_w) % tex_w;
        const int ty = (int)(ty_float * tex_h) % tex_h;

        color_t c = texture_get_pixel_at(&rayc->textures[pt], tx, ty);

        // Apply shading to floors too based on distance
        // color_sub(&c, (int)wy / SHADING_SCALE);

        gfx_draw_pixel(x, screen_y, c);
      }
    }
  }
}

static void draw_wall(
  rayc_t * rayc,
  int x1, int x2,
  int b1, int b2,
  int t1, int t2,
  int s,  int w,
  int face,
  float z0, float z1,
  float u0, float u1
) {
  const map_t * m = &rayc->map;

  sector_runtime_t * rt_sec = &rayc->sector_rt[s];

  const int shade = calculate_shade(&m->walls[w]);

  const int wt = m->walls[w].texture;

  // Division variables for perspective correction
  const float iz0 = 1.0f / z0;
  const float iz1 = 1.0f / z1;
  const float uz0 = u0 / z0;
  const float uz1 = u1 / z1;

  const int dyb = b2 - b1;
  const int dyt = t2 - t1;
  int dx = x2 - x1;
  if (dx == 0) { dx = 1; }

  const int x1orig = x1; // Store original starting x before clipping

  if (x1 < 0) { x1 = 0; }
  if (x2 < 0) { x2 = 0; }
  if (x1 > WIDTH) { x1 = WIDTH; }
  if (x2 > WIDTH) { x2 = WIDTH; }

  for (int x = x1; x < x2; ++x) {
    // Interpolation factor t goes from 0.0 to 1.0 across the unclipped wall width
    float t = (float)(x - x1orig) / (float)dx;

    // Perspective-correct texture coordinate interpolation
    float iz = iz0 + t * (iz1 - iz0);
    float uz = uz0 + t * (uz1 - uz0);
    float ht = (uz / iz) * (float)rayc->textures[wt].width;

    int y_bot = dyb * (x - x1orig + 0.5f) / dx + b1;
    int y_top = dyt * (x - x1orig + 0.5f) / dx + t1;

    float vt = 0; // Vertical texture coordinate
    const float vt_step = (float)rayc->textures[wt].height * m->walls[w].v / (float)(MAX(y_top, y_bot) - MIN(y_top, y_bot));

    // Offset vt init value by how much texture is actually visible on screen (vertical look down)
    if (MIN(y_top, y_bot) < 0) {
      vt += vt_step * (float)(-MIN(y_top, y_bot));
    }

    if (y_bot < 0) { vt -= vt_step * y_bot; y_bot = 0; }
    if (y_top < 0) { y_top = 0; }
    if (y_bot > HEIGHT) { y_bot = HEIGHT; }
    if (y_top > HEIGHT) { y_top = HEIGHT; }

    const int surf_mode = rt_sec->surface;

    int y_start = MIN(y_top, y_bot);
    int y_end   = MAX(y_top, y_bot);

    if (face == 0) { // Walls
      if (surf_mode == 1) {
        rt_sec->surf[x] = y_bot;
      }

      if (surf_mode == 2) {
        rt_sec->surf[x] = y_top;
      }

      // Calculate the wall depth for this exact column
      const float wall_depth = 1.0f / iz;

      for (int y = y_start; y < y_end; ++y) {
        int buf_idx = x + y * WIDTH;

        // Only draw if it's closer than what's currently on screen
        if (wall_depth < rayc->z_buffer[buf_idx]) {
          rayc->z_buffer[buf_idx] = wall_depth;

          color_t c = texture_get_pixel_at(&rayc->textures[wt], (int)ht % rayc->textures[wt].width, (int)vt % rayc->textures[wt].height);
          color_sub(&c, shade / SHADING_SCALE);
          gfx_draw_pixel(x, y, c);
        }
        vt += vt_step;
      }
    } else if (face == 1) { // Floors / Ceilings
      draw_floor_part(rayc, x, s, y_start, y_end, wt);
    }
  }
}

void rayc_draw_scene(rayc_t * rayc) {
  const player_t * p = &rayc->player;
  const map_t * m = &rayc->map;

  ASSERT_RET(m->sectors && m->walls);

  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    rayc->z_buffer[i] = 999999.0f;
  }

  uint16_t render_order[MAX_SECTORS];
  int visible_count = 0;

  for (int s = 0; s < m->sector_count; ++s) {
    render_order[s] = s;
  }

  // Find which sectors to actually draw
  for (int s = 0; s < m->sector_count; ++s) {
    // 1. Calculate true 2D bounding box center
    int min_x = INT16_MAX, max_x = INT16_MIN;
    int min_y = INT16_MAX, max_y = INT16_MIN;

    for (int w = m->sectors[s].ws; w < m->sectors[s].we; ++w) {
      if (m->walls[w].x1 < min_x) min_x = m->walls[w].x1;
      if (m->walls[w].x1 > max_x) max_x = m->walls[w].x1;
      if (m->walls[w].y1 < min_y) min_y = m->walls[w].y1;
      if (m->walls[w].y1 > max_y) max_y = m->walls[w].y1;
    }

    int cx = (min_x + max_x) / 2;
    int cy = (min_y + max_y) / 2;
    int cz = (m->sectors[s].z1 + m->sectors[s].z2) / 2;

    // 2. Calculate distance with heavily weighted Z-axis
    float dx = (float)(cx - p->pos.x);
    float dy = (float)(cy - p->pos.y);
    float dz = (float)(cz - p->pos.z);

    // Multiplying dz*dz artificially forces vertical layers to be prioritized
    // in the Painter's sort, preventing large lower sectors from popping over small top sectors
    float z_weight = 4.0f;
    int dist = (int)(dx * dx + dy * dy + (dz * dz * z_weight));

    if (dist > (MAX_DRAW_DIST * MAX_DRAW_DIST)) continue;

    if (visible_count < MAX_SECTORS) {
      rayc->sector_rt[visible_count].map_sector_idx = s;
      rayc->sector_rt[visible_count].distance = dist;
      render_order[visible_count] = visible_count;
      visible_count++;
    }
  }

  // Order visible sectors by distance (Painter's Algorithm)
  for (int i = 0; i < visible_count - 1; ++i) {
    for (int j = 0; j < visible_count - i - 1; ++j) {
      uint16_t idx_a = render_order[j];
      uint16_t idx_b = render_order[j + 1];

      if (rayc->sector_rt[idx_a].distance < rayc->sector_rt[idx_b].distance) {
        SWAP(render_order[j], render_order[j + 1]);
      }
    }
  }

  // Draw sectors
  for (int i = 0; i < visible_count; ++i) {
    int rt_idx = render_order[i];

    sector_runtime_t * rt_sec = &rayc->sector_rt[rt_idx];
    sector_t * map_sec = &m->sectors[rt_sec->map_sector_idx];

    int face_cycles = 0;

    // m->sectors[s].d = 0;
    rt_sec->distance = 0;

    if (p->pos.z < map_sec->z1) {
      rt_sec->surface = 1; // Bottom surface (floor)
      face_cycles = 2;

      for (int i = 0; i < WIDTH; ++i) {
        rt_sec->surf[i] = HEIGHT;
      }
    } else if (p->pos.z > map_sec->z2) {
      rt_sec->surface = 2; // Top surface (ceiling)
      face_cycles = 2;

      // memset(map_sec->surf, 0, WIDTH);
      for (int i = 0; i < WIDTH; ++i) {
        rt_sec->surf[i] = 0;
      }
    } else {
      rt_sec->surface = 0; // No surface
      face_cycles = 1;
    }

    for (int face = 0; face < face_cycles; ++face) {
      for (int w = map_sec->ws; w < map_sec->we; ++w) {
        int wx[4], wy[4], wz[4];

        float pa_cos = fast_cos(p->angle);
        float pa_sin = fast_sin(p->angle);

        int x1 = m->walls[w].x1 - p->pos.x;
        int y1 = m->walls[w].y1 - p->pos.y;
        int x2 = m->walls[w].x2 - p->pos.x;
        int y2 = m->walls[w].y2 - p->pos.y;

        // Set up initial U coordinates based on the wall texture scale
        float u0 = 0.0f;
        float u1 = m->walls[w].u;

        if (face) {
          SWAP(x1, x2);
          SWAP(y1, y2);
          SWAP(u0, u1); // Swap u coords if drawing a backface so the texture isn't reversed
        }

        // World X
        wx[0] = x1 * pa_cos - y1 * pa_sin;
        wx[1] = x2 * pa_cos - y2 * pa_sin;
        wx[2] = wx[0];
        wx[3] = wx[1];

        // World Y (depth)
        wy[0] = y1 * pa_cos + x1 * pa_sin;
        wy[1] = y2 * pa_cos + x2 * pa_sin;
        wy[2] = wy[0];
        wy[3] = wy[1];

        // Store wall distance
        rt_sec->distance += distance(0, 0, (wx[0] + wx[1]) / 2, (wy[0] + wy[1]) / 2);

        // World Z (height)
        wz[0] = map_sec->z1 - p->pos.z + ((p->look * wy[0]) / H_LOOK_SCALE);
        wz[1] = map_sec->z1 - p->pos.z + ((p->look * wy[1]) / H_LOOK_SCALE);
        // wz[2] = wz[0] + map_sec->z2;
        // wz[3] = wz[1] + map_sec->z2;
        wz[2] = map_sec->z2 - p->pos.z + ((p->look * wy[0]) / H_LOOK_SCALE);
        wz[3] = map_sec->z2 - p->pos.z + ((p->look * wy[1]) / H_LOOK_SCALE);

        if (wy[0] < 1 && wy[1] < 1) { continue; }

        if (wy[0] < 1) {
          clip_behind_camera(&wx[0], &wy[0], &wz[0], &u0, wx[1], wy[1], wz[1], u1);
          clip_behind_camera(&wx[2], &wy[2], &wz[2], NULL, wx[3], wy[3], wz[3], 0);
        }

        if (wy[1] < 1) {
          clip_behind_camera(&wx[1], &wy[1], &wz[1], &u1, wx[0], wy[0], wz[0], u0);
          clip_behind_camera(&wx[3], &wy[3], &wz[3], NULL, wx[2], wy[2], wz[2], 0);
        }

        // Store actual depths for perspective-correct texture mapping
        float z0 = (float)wy[0];
        float z1 = (float)wy[1];

        // Screen X / Y position flipped for SDL2 top left origin
        wx[0] = wx[0] * FOV_MODIFIER / wy[0] + WIDTH / 2;
        wy[0] = HEIGHT / 2 - wz[0] * FOV_MODIFIER / wy[0];

        wx[1] = wx[1] * FOV_MODIFIER / wy[1] + WIDTH / 2;
        wy[1] = HEIGHT / 2 - wz[1] * FOV_MODIFIER / wy[1];

        wx[2] = wx[2] * FOV_MODIFIER / wy[2] + WIDTH / 2;
        wy[2] = HEIGHT / 2 - wz[2] * FOV_MODIFIER / wy[2];

        wx[3] = wx[3] * FOV_MODIFIER / wy[3] + WIDTH / 2;
        wy[3] = HEIGHT / 2 - wz[3] * FOV_MODIFIER / wy[3];

        draw_wall(rayc, wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], rt_idx, w, face, z0, z1, u0, u1);
      }

      const int num_walls = map_sec->we - map_sec->ws;

      if (num_walls > 0) {
        rt_sec->distance /= num_walls;
      }
    }
  }
}
