#pragma once

#include "rayc/defs.h"

#if WITH_EDITOR

typedef struct rayc_s rayc_t;

typedef enum {
  EDITOR_MODE_SELECT,
  EDITOR_MODE_DRAW,
  EDITOR_MODE_DELETE,

  __EDITOR_MODE_MAX,
} editor_mode_t;

typedef struct {
  bool enabled;

  editor_mode_t mode;

  struct {
    float x, y;
    float zoom;

    // Selection state
    int selected_sector;
    int selected_wall;
    int selected_vertex; // 0 = none, 1 = p1 (x1,y1), 2 = p2 (x2,y2)

    // Mouse state
    bool is_dragging;
    int  mouse_x, mouse_y;
    int  prev_mouse_x, prev_mouse_y;
  } edit_cam;

  struct {
    int x[EDITOR_MAX_POINTS];
    int y[EDITOR_MAX_POINTS];
    int count;
  } draw_pts;
} editor_t;

void rayc_draw_editor(rayc_t * rayc);
void rayc_process_editor_input(rayc_t * rayc);

#endif
