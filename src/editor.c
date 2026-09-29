#include "rayc/editor.h"
#include "rayc/rayc.h"
#include "rayc/gfx.h"
#include <stdio.h>

#if WITH_EDITOR

#define GUI_STEPPER_DEF(__type, __suffix, __fmt)                        \
  static void gui_stepper_ ## __suffix(                                 \
      gui_ctx_t *    ctx,                                               \
      const rayc_t * rayc,                                              \
      const char *   label,                                             \
      __type *       val,                                               \
      const __type   step,                                              \
      const __type   min,                                               \
      const __type   max                                                \
  ) {                                                                   \
    int h = 14;                                                         \
    int btn_w = 14;                                                     \
    int spacing = 2;                                                    \
                                                                        \
    /* Calculate Label Width using font.h */                            \
    int label_w = font_calc_str_width(rayc->font, label);               \
                                                                        \
    /* Right-side anchors for buttons */                                \
    int right_edge = ctx->cursor_x + ctx->item_width;                   \
    int plus_x  = right_edge - btn_w;                                   \
    int minus_x = plus_x - btn_w - spacing;                             \
                                                                        \
    /* Fluid bounds for the value box */                                \
    int pad = (label_w > 0) ? 2 : 0;                                    \
    int val_x = ctx->cursor_x + label_w + pad;                          \
    int val_w = minus_x - val_x - spacing;                              \
                                                                        \
    char buf[32];                                                       \
                                                                        \
    /* Render Label */                                                  \
    gfx_draw_string(rayc->font, ctx->cursor_x, ctx->cursor_y + 3,       \
                    RGB(200, 200, 200), label);                         \
                                                                        \
    /* Render Value Box (only if there is room) */                      \
    if (val_w > 0) {                                                    \
        gfx_fill_rect(val_x, ctx->cursor_y, val_w, h, RGB(30, 30, 30)); \
        gfx_draw_rect(val_x, ctx->cursor_y, val_w, h, RGB(80, 80, 80)); \
    }                                                                   \
                                                                        \
    /* Render Value Text */                                             \
    snprintf(buf, sizeof(buf), __fmt, *val);                            \
    gfx_draw_string(rayc->font, val_x + 3, ctx->cursor_y + 3,           \
                    RGB(255, 255, 255), buf);                           \
                                                                        \
    /* Render Buttons */                                                \
    if (gui_button(ctx, rayc, minus_x, ctx->cursor_y, btn_w, h, "-")    \
      && *val - step >= min) {                                          \
        *val -= step;                                                   \
    }                                                                   \
    if (gui_button(ctx, rayc, plus_x,  ctx->cursor_y, btn_w, h, "+")    \
      && *val + step <= max) {                                          \
        *val += step;                                                   \
    }                                                                   \
                                                                        \
    /* Advance global layout cursor */                                  \
    ctx->cursor_y += h + 4;                                             \
  }

typedef struct {
  int  mx, my;
  bool left_down;
  bool left_pressed;

  // Layout State
  int cursor_x;
  int cursor_y;
  int item_width;
} gui_ctx_t;

// Helper to initialize the panel's layout area
static void gui_layout_begin(gui_ctx_t * ctx, int x, int y, int w) {
  ctx->cursor_x = x;
  ctx->cursor_y = y;
  ctx->item_width = w;
}

// Helper for manual spacing (e.g., between sections)
static void gui_layout_advance(gui_ctx_t * ctx, int h) {
  ctx->cursor_y += h;
}

static bool gui_button(
  const gui_ctx_t * ctx,
  const rayc_t *    rayc,
  const int         x,
  const int         y,
  const int         w,
  const int         h,
  const char *      label
) {
  bool hovered = (ctx->mx >= x && ctx->mx < x + w && ctx->my >= y && ctx->my < y + h);
  color_t bg = RGB(60, 60, 60);

  if (hovered) {
    bg = ctx->left_down ? RGB(100, 100, 100) : RGB(80, 80, 80);
  }

  gfx_fill_rect(x, y, w, h, bg);
  gfx_draw_rect(x, y, w, h, RGB(150, 150, 150));
  gfx_draw_string(rayc->font, x + 3, y + (h / 2) - 3, RGB(255, 255, 255), label);

  return hovered && ctx->left_pressed;
}

static void gui_draw_texture_preview(rayc_t * rayc, int x, int y, int size, uint8_t tex_idx) {
  // Draw border
  gfx_draw_rect(x - 1, y - 1, size + 2, size + 2, RGB(150, 150, 150));

  texture_t * tex = &rayc->textures[tex_idx];

  // Draw nearest-neighbor scaled texture
  if (tex->data && tex->width > 0 && tex->height > 0) {
    for (int py = 0; py < size; ++py) {
      for (int px = 0; px < size; ++px) {
        int tex_x = (px * tex->width) / size;
        int tex_y = (py * tex->height) / size;
        gfx_draw_pixel(x + px, y + py, texture_get_pixel_at(tex, tex_x, tex_y));
      }
    }
  } else {
    // Missing texture fallback
    gfx_fill_rect(x, y, size, size, RGB(80, 80, 80));
    gfx_draw_string(rayc->font, x + (size / 2) - 12, y + (size / 2) - 4, RGB(255, 0, 0), "?");
  }
}

GUI_STEPPER_DEF(int16_t, i16, "%d")
GUI_STEPPER_DEF(uint8_t, u8,  "%u")
GUI_STEPPER_DEF(int8_t,  i8,  "%d")

static void gui_texture_stepper_with_preview(
  gui_ctx_t *  ctx,
  rayc_t *     rayc,
  const char * label,
  uint8_t *    val,
  const int    preview_size
) {
  const int start_y = ctx->cursor_y;

  // Draw image on the left
  gui_draw_texture_preview(rayc, ctx->cursor_x, start_y, preview_size, *val);

  // Save horizontal bounds
  int offset_x = preview_size + 4;
  int orig_x = ctx->cursor_x;
  int orig_w = ctx->item_width;

  // Shift bounds right, and vertically center the stepper (assuming ~14px tall)
  ctx->cursor_x += offset_x;
  ctx->item_width -= offset_x;
  ctx->cursor_y += (preview_size / 2) - 7;

  gui_stepper_u8(ctx, rayc, label, val, 1, 0, UINT8_MAX);

  // Restore bounds and advance Y past the preview box
  ctx->cursor_x = orig_x;
  ctx->item_width = orig_w;
  ctx->cursor_y = start_y + preview_size + 8;
}

// World to Screen
static int ed_w2s_x(rayc_t * rayc, float wx) {
  return (int)((wx - rayc->editor.edit_cam.x) * rayc->editor.edit_cam.zoom + (WIDTH / 2));
}

static int ed_w2s_y(rayc_t * rayc, float wy) {
  return (int)((wy - rayc->editor.edit_cam.y) * rayc->editor.edit_cam.zoom + (HEIGHT / 2));
}

// Screen to World
static float ed_s2w_x(rayc_t * rayc, int sx) {
  return ((float)(sx - (WIDTH / 2)) / rayc->editor.edit_cam.zoom) + rayc->editor.edit_cam.x;
}

static float ed_s2w_y(rayc_t * rayc, int sy) {
  return ((float)(sy - (HEIGHT / 2)) / rayc->editor.edit_cam.zoom) + rayc->editor.edit_cam.y;
}

static void draw_draw_mode(rayc_t * rayc) {
  int pts = rayc->editor.draw_pts.count;
  color_t draw_col = RGB(0, 255, 0); // Green for drawing

  // Draw finalized lines of the current shape
  for (int i = 0; i < pts - 1; ++i) {
    int sx1 = ed_w2s_x(rayc, rayc->editor.draw_pts.x[i]);
    int sy1 = ed_w2s_y(rayc, rayc->editor.draw_pts.y[i]);
    int sx2 = ed_w2s_x(rayc, rayc->editor.draw_pts.x[i+1]);
    int sy2 = ed_w2s_y(rayc, rayc->editor.draw_pts.y[i+1]);
    gfx_draw_line(sx1, sy1, sx2, sy2, draw_col);
  }

  // Draw the "rubber band" line from the last point to the snapped mouse cursor
  if (pts > 0) {
    float world_mx = ed_s2w_x(rayc, rayc->input.mouse.x);
    float world_my = ed_s2w_y(rayc, rayc->input.mouse.y);

    float snap = MAP_GRID_SIZE;
    int snapped_x = round(world_mx / snap) * snap;
    int snapped_y = round(world_my / snap) * snap;

    int sx1 = ed_w2s_x(rayc, rayc->editor.draw_pts.x[pts-1]);
    int sy1 = ed_w2s_y(rayc, rayc->editor.draw_pts.y[pts-1]);
    int sx2 = ed_w2s_x(rayc, snapped_x);
    int sy2 = ed_w2s_y(rayc, snapped_y);

    // Snapping feedback: Turn line yellow if hovering over the starting point
    float d = distance(world_mx, world_my, rayc->editor.draw_pts.x[0], rayc->editor.draw_pts.y[0]);
    if (pts >= 3 && d < (16.0f / rayc->editor.edit_cam.zoom)) {
      sx2 = ed_w2s_x(rayc, rayc->editor.draw_pts.x[0]);
      sy2 = ed_w2s_y(rayc, rayc->editor.draw_pts.y[0]);
      draw_col = RGB(255, 255, 0);
    }

    gfx_draw_line(sx1, sy1, sx2, sy2, draw_col);
  }

  // Draw vertex nodes
  for (int i = 0; i < pts; ++i) {
    int sx = ed_w2s_x(rayc, rayc->editor.draw_pts.x[i]);
    int sy = ed_w2s_y(rayc, rayc->editor.draw_pts.y[i]);
    gfx_draw_rect(sx - 3, sy - 3, 6, 6, RGB(0, 255, 0));
  }
}

static void draw_sector_edit_panel(rayc_t * rayc) {
  int pw = EDITOR_WIDTH;
  int px = WIDTH - pw;
  int py = 0;

  // Draw Panel Background
  gfx_fill_rect(px, py, pw, HEIGHT, RGB(80, 80, 80));
  gfx_draw_line(px, py, px, HEIGHT, RGB(100, 100, 100));

  // Setup UI Context
  static bool prev_ui_click = false;
  gui_ctx_t ui = {
    .mx = rayc->input.mouse.x,
    .my = rayc->input.mouse.y,
    .left_down = rayc->input.mouse.left_btn,
    .left_pressed = rayc->input.mouse.left_btn && !prev_ui_click
  };
  prev_ui_click = rayc->input.mouse.left_btn;

  // Initialize layout with a 5px padding on the left/right
  gui_layout_begin(&ui, px + 5, 5, pw - 10);

  int sec_idx = rayc->editor.edit_cam.selected_sector;
  sector_t * sec = &rayc->map.sectors[sec_idx];

  // Sector Properties
  gfx_draw_string(rayc->font, ui.cursor_x, ui.cursor_y, RGB(255, 255, 0), "SECTOR");
  gui_layout_advance(&ui, 12); // Space after header

  gui_stepper_i16(&ui, rayc, "z1", &sec->z1, 8, INT16_MIN, INT16_MAX);
  gui_stepper_i16(&ui, rayc, "z2", &sec->z2, 8, INT16_MIN, INT16_MAX);
  gui_texture_stepper_with_preview(&ui, rayc, "tf", &sec->tf, 24);
  gui_texture_stepper_with_preview(&ui, rayc, "tc", &sec->tc, 24);

  // Wall Properties (if a specific wall is selected)
  int wall_idx = rayc->editor.edit_cam.selected_wall;
  if (wall_idx != -1) {
    wall_t * wall = &rayc->map.walls[wall_idx];

    gfx_draw_string(rayc->font, ui.cursor_x, ui.cursor_y, RGB(0, 255, 255), "WALL");
    gui_layout_advance(&ui, 12);

    gui_texture_stepper_with_preview(&ui, rayc, "wt", &wall->texture, 24);

    gui_stepper_i8(&ui, rayc, "u", &wall->u, 1, 1, INT8_MAX);
    gui_stepper_i8(&ui, rayc, "v", &wall->v, 1, 1, INT8_MAX);
  }
}

void rayc_draw_editor(rayc_t * rayc) {
  const map_t * m = &rayc->map;

  // Draw a background grid (optional but highly recommended)
  float start_x = floorf(ed_s2w_x(rayc, 0) / MAP_GRID_SIZE) * MAP_GRID_SIZE;
  float start_y = floorf(ed_s2w_y(rayc, 0) / MAP_GRID_SIZE) * MAP_GRID_SIZE;

  for (float gx = start_x; ed_w2s_x(rayc, gx) < WIDTH; gx += MAP_GRID_SIZE) {
    gfx_draw_line(ed_w2s_x(rayc, gx), 0, ed_w2s_x(rayc, gx), HEIGHT, RGB(40, 40, 40));
  }
  for (float gy = start_y; ed_w2s_y(rayc, gy) < HEIGHT; gy += MAP_GRID_SIZE) {
    gfx_draw_line(0, ed_w2s_y(rayc, gy), WIDTH, ed_w2s_y(rayc, gy), RGB(40, 40, 40));
  }

  ASSERT_RET(m->sectors && m->walls);

  // Draw Sectors and Walls
  for (int s = 0; s < m->sector_count; ++s) {
    sector_t * sec = &m->sectors[s];
    bool sector_selected = (rayc->editor.edit_cam.selected_sector == s);

    for (int w = sec->ws; w < sec->we; ++w) {
      wall_t * wall = &m->walls[w];
      bool wall_selected = (rayc->editor.edit_cam.selected_wall == w);

      int sx1 = ed_w2s_x(rayc, wall->x1);
      int sy1 = ed_w2s_y(rayc, wall->y1);
      int sx2 = ed_w2s_x(rayc, wall->x2);
      int sy2 = ed_w2s_y(rayc, wall->y2);

      // Determine colors based on selection
      color_t wall_col = wall_selected ? COLOR_YELLOW : (sector_selected ? RGB(200, 200, 200) : RGB(100, 100, 100));

      // Draw Wall
      gfx_draw_line(sx1, sy1, sx2, sy2, wall_col);

      // Draw Normal (perpendicular line to show which way the wall faces)
      float dx = wall->x2 - wall->x1;
      float dy = wall->y2 - wall->y1;
      float len = sqrtf(dx*dx + dy*dy);

      if (len > 0) {
        // Invert the signs here to flip the normal 180 degrees
        float nx = dy / len * 5.0f;
        float ny = -dx / len * 5.0f;

        int mx = ed_w2s_x(rayc, wall->x1 + dx * 0.5f);
        int my = ed_w2s_y(rayc, wall->y1 + dy * 0.5f);

        gfx_draw_line(mx, my, ed_w2s_x(rayc, wall->x1 + dx * 0.5f + nx), ed_w2s_y(rayc, wall->y1 + dy * 0.5f + ny), COLOR_CYAN);
      }

      // Draw Vertices
      color_t v1_col = (wall_selected && rayc->editor.edit_cam.selected_vertex == 1) ? COLOR_GREEN : COLOR_RED;
      color_t v2_col = (wall_selected && rayc->editor.edit_cam.selected_vertex == 2) ? COLOR_GREEN : COLOR_RED;

      gfx_draw_rect(sx1 - 3, sy1 - 3, 6, 6, v1_col);
      gfx_draw_rect(sx2 - 3, sy2 - 3, 6, 6, v2_col);
    }
  }

  if (rayc->editor.mode == EDITOR_MODE_DRAW) {
    draw_draw_mode(rayc);
  }

  if (rayc->editor.mode == EDITOR_MODE_SELECT && rayc->editor.edit_cam.selected_sector != -1) {
    draw_sector_edit_panel(rayc);
  }
}

static void process_draw_mode(
  rayc_t *    rayc,
  const bool  left_pressed,
  const float world_mx,
  const float world_my,
  const int   snapped_x,
  const int   snapped_y
) {
  if (!left_pressed) {
    return;
  }

  int pts = rayc->editor.draw_pts.count;

  // Check if clicking near the FIRST point to close the loop
  if (pts >= 3) {
    float d = distance(world_mx, world_my, rayc->editor.draw_pts.x[0], rayc->editor.draw_pts.y[0]);
    if (d < (16.0f / rayc->editor.edit_cam.zoom)) {

      // Close the loop & create the sector
      int new_sec_idx = map_add_sector(&rayc->map, 0, 32);

      for (int i = 0; i < pts; ++i) {
        wall_t new_wall;

        // Flip order: point (i+1) is now the start, and Point (i) is now the end
        new_wall.x1 = rayc->editor.draw_pts.x[(i + 1) % pts];
        new_wall.y1 = rayc->editor.draw_pts.y[(i + 1) % pts];
        new_wall.x2 = rayc->editor.draw_pts.x[i];
        new_wall.y2 = rayc->editor.draw_pts.y[i];

        new_wall.texture = 0;
        new_wall.u = 1;
        new_wall.v = 1;

        map_insert_wall(&rayc->map, new_sec_idx, i, new_wall);
      }

      rayc->editor.draw_pts.count = 0;
      return; // Skip rest of frame to avoid double-processing
    }
  }

  // Add a new point to the buffer (limit 64 points)
  if (pts < EDITOR_MAX_POINTS) {
    rayc->editor.draw_pts.x[pts] = snapped_x;
    rayc->editor.draw_pts.y[pts] = snapped_y;
    rayc->editor.draw_pts.count++;
  }
}

static void process_select_mode(
  rayc_t *    rayc,
  const bool  left_down,
  const float world_mx,
  const float world_my,
  const int   snapped_x,
  const int   snapped_y
) {
  // Handle Selection on Click
  if (left_down && !rayc->editor.edit_cam.is_dragging) {
    rayc->editor.edit_cam.is_dragging = true;

    float closest_dist = 10.0f / rayc->editor.edit_cam.zoom; // 10 screen-pixel snap radius

    // Store the previous selection to detect when we should cycle
    int prev_sel_wall = rayc->editor.edit_cam.selected_wall;

    rayc->editor.edit_cam.selected_wall = -1;
    rayc->editor.edit_cam.selected_vertex = 0;

    for (int s = 0; s < rayc->map.sector_count; ++s) {
      for (int w = rayc->map.sectors[s].ws; w < rayc->map.sectors[s].we; ++w) {
        wall_t * wall = &rayc->map.walls[w];

        // Check vertex 1
        float d1 = distance(world_mx, world_my, wall->x1, wall->y1);
        if (d1 <= closest_dist) {
          // Take it if it's strictly closer, OR if it's an exact overlap tie
          // and the current winner is the wall we had selected last frame (forces a cycle).
          if (d1 < closest_dist || rayc->editor.edit_cam.selected_wall == prev_sel_wall) {
            closest_dist = d1;
            rayc->editor.edit_cam.selected_sector = s;
            rayc->editor.edit_cam.selected_wall = w;
            rayc->editor.edit_cam.selected_vertex = 1;
          }
        }

        // Check vertex 2
        float d2 = distance(world_mx, world_my, wall->x2, wall->y2);
        if (d2 <= closest_dist) {
          if (d2 < closest_dist || rayc->editor.edit_cam.selected_wall == prev_sel_wall) {
            closest_dist = d2;
            rayc->editor.edit_cam.selected_sector = s;
            rayc->editor.edit_cam.selected_wall = w;
            rayc->editor.edit_cam.selected_vertex = 2;
          }
        }
      }
    }
  }

  // Handle Dragging (with linked vertices)
  if (left_down && rayc->editor.edit_cam.selected_wall != -1 && rayc->editor.edit_cam.selected_vertex != 0) {
    wall_t * sel_wall = &rayc->map.walls[rayc->editor.edit_cam.selected_wall];

    int orig_x = (rayc->editor.edit_cam.selected_vertex == 1) ? sel_wall->x1 : sel_wall->x2;
    int orig_y = (rayc->editor.edit_cam.selected_vertex == 1) ? sel_wall->y1 : sel_wall->y2;

    // Only do the work if we actually moved to a new grid coordinate
    if (snapped_x != orig_x || snapped_y != orig_y) {

      // NEW: Check if the target position is already occupied by any vertex
      bool position_occupied = false;
      for (int w = 0; w < rayc->map.wall_count; ++w) {
        if ((rayc->map.walls[w].x1 == snapped_x && rayc->map.walls[w].y1 == snapped_y) ||
            (rayc->map.walls[w].x2 == snapped_x && rayc->map.walls[w].y2 == snapped_y)) {
          position_occupied = true;
          break;
        }
      }

      // Only update if the spot is free (this also prevents 0-length walls!)
      if (!position_occupied) {
        for (int w = 0; w < rayc->map.wall_count; ++w) {
          if (rayc->map.walls[w].x1 == orig_x && rayc->map.walls[w].y1 == orig_y) {
            rayc->map.walls[w].x1 = snapped_x;
            rayc->map.walls[w].y1 = snapped_y;
          }
          if (rayc->map.walls[w].x2 == orig_x && rayc->map.walls[w].y2 == orig_y) {
            rayc->map.walls[w].x2 = snapped_x;
            rayc->map.walls[w].y2 = snapped_y;
          }
        }
      }
    }
  }

  if (!left_down) {
    rayc->editor.edit_cam.is_dragging = false;
  }
}

static void process_delete_mode(
  rayc_t *    rayc,
  const bool  left_pressed,
  const float world_mx,
  const float world_my
) {
  if (!left_pressed) {
    return;
  }

  float closest_dist = 10.0f / rayc->editor.edit_cam.zoom;
  int target_sector = -1;

  // Find the sector under the cursor by checking its vertices
  for (int s = 0; s < rayc->map.sector_count; ++s) {
    for (int w = rayc->map.sectors[s].ws; w < rayc->map.sectors[s].we; ++w) {
      wall_t * wall = &rayc->map.walls[w];

      float d1 = distance(world_mx, world_my, wall->x1, wall->y1);
      float d2 = distance(world_mx, world_my, wall->x2, wall->y2);

      if (d1 < closest_dist || d2 < closest_dist) {
        closest_dist = MIN(d1, d2);
        target_sector = s;
      }
    }
  }

  // If we clicked a valid sector, obliterate it
  if (target_sector != -1) {
    map_delete_sector(&rayc->map, target_sector);

    // Clear selection state just in case we deleted the currently selected item
    rayc->editor.edit_cam.selected_sector = -1;
    rayc->editor.edit_cam.selected_wall = -1;
    rayc->editor.edit_cam.selected_vertex = 0;
  }
}

void rayc_process_editor_input(rayc_t * rayc) {
  int mx = rayc->input.mouse.x;
  int my = rayc->input.mouse.y;

  bool left_down = rayc->input.mouse.left_btn;
  bool right_down = rayc->input.mouse.right_btn;

  // Edge detection for drawing clicks (so we don't place multiple points per frame)
  static bool prev_left_down = false;
  bool left_pressed = left_down && !prev_left_down;
  prev_left_down = left_down;

  bool selecting = (rayc->editor.edit_cam.selected_sector != -1);
  int panel_x = WIDTH - EDITOR_WIDTH;

  if (selecting && mx >= panel_x) {
    left_down = false;
    left_pressed = false;
    right_down = false;
  }

  float world_mx = ed_s2w_x(rayc, mx);
  float world_my = ed_s2w_y(rayc, my);

  // Grid snapping
  float snap = MAP_GRID_SIZE;
  int snapped_x = round(world_mx / snap) * snap;
  int snapped_y = round(world_my / snap) * snap;

  // Switch mode with space
  if (rayc->input.kb[SCANCODE_SPACE].pressed) {
    rayc->editor.mode = (rayc->editor.mode + 1) % __EDITOR_MODE_MAX;
    rayc->editor.draw_pts.count = 0; // Reset drawing buffer on mode switch
  }

  if (rayc->map.sectors && rayc->map.walls) {
    switch (rayc->editor.mode) {
      case EDITOR_MODE_DRAW: {
        process_draw_mode(rayc, left_pressed, world_mx, world_my, snapped_x, snapped_y);
        break;
      }

      case EDITOR_MODE_SELECT: {
        process_select_mode(rayc, left_down, world_mx, world_my, snapped_x, snapped_y);
        break;
      }

      case EDITOR_MODE_DELETE: {
        process_delete_mode(rayc, left_pressed, world_mx, world_my);
        break;
      }

      default: {
        rayc->editor.mode = EDITOR_MODE_DRAW;
        break;
      }
    }
  }

  // Handle Camera Panning (Right click drag)
  if (right_down) {
    rayc->editor.edit_cam.x -= (mx - rayc->editor.edit_cam.prev_mouse_x) / rayc->editor.edit_cam.zoom;
    rayc->editor.edit_cam.y -= (my - rayc->editor.edit_cam.prev_mouse_y) / rayc->editor.edit_cam.zoom;
  }

  if (rayc->input.kb[SCANCODE_UP].held)    { rayc->editor.edit_cam.y -= EDITOR_ARROWS_MOVE_SPEED; }
  if (rayc->input.kb[SCANCODE_DOWN].held)  { rayc->editor.edit_cam.y += EDITOR_ARROWS_MOVE_SPEED; }
  if (rayc->input.kb[SCANCODE_LEFT].held)  { rayc->editor.edit_cam.x -= EDITOR_ARROWS_MOVE_SPEED; }
  if (rayc->input.kb[SCANCODE_RIGHT].held) { rayc->editor.edit_cam.x += EDITOR_ARROWS_MOVE_SPEED; }

  // Handle Zoom (Using Keyboard since wheel isn't in input_t)
  if (rayc->input.kb[SCANCODE_EQUALS].held) {
    rayc->editor.edit_cam.zoom *= 1.05f; // Zoom in
  }
  if (rayc->input.kb[SCANCODE_MINUS].held) {
    rayc->editor.edit_cam.zoom *= 0.95f; // Zoom out
  }

  // Store mouse state for the next frame's delta calculations
  rayc->editor.edit_cam.prev_mouse_x = mx;
  rayc->editor.edit_cam.prev_mouse_y = my;
}

#endif
