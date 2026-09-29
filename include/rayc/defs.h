#pragma once

#define FPS_LOCK 30

// #define SCALE  6
// #define WIDTH  160
// #define HEIGHT 120

#define SCALE  3
#define WIDTH  320
#define HEIGHT 240

// #define FOV 90
#define FOV_MODIFIER 200
#define H_LOOK_SCALE 32.0f
#define V_LOOK_RATE  2
#define TURN_RATE    4
#define MOVE_RATE    10

#define SHADING_SCALE 6

#define MAX_DRAW_DIST 1000

#define MAP_GRID_SIZE 32

#define MAX_SECTORS  32
#define MAX_WALLS    32
#define MAX_FILES    32
#define MAX_TEXTURES 32

#define MAX_FILENAME 32

#define CONSOLE_SIZE_PERCENT  80
#define CONSOLE_LINE_BUF_SIZE 10
#define CONSOLE_LINE_MAX      32
#define CONSOLE_ARGV_MAX      8
#define CONSOLE_ARG_MAX       16
#define CONSOLE_PROMPT        ">"

#define CONSOLE_COMMANDS \
  X(exit,   "Exit the game",        builtin_cmd_exit)   \
  X(help,   "Get cmd help",         builtin_cmd_help)   \
  X(map,    "Map control",          builtin_cmd_map)    \
  X(resmap, "Resource map control", builtin_cmd_resmap) \
  X(test,   "Test",                 builtin_cmd_test)

#define WITH_EDITOR 1

#define EDITOR_WIDTH             100
#define EDITOR_MAX_POINTS        64
#define EDITOR_ARROWS_MOVE_SPEED 16
