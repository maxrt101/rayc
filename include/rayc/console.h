#pragma once

#include "rayc/defs.h"

typedef struct rayc_s rayc_t;

typedef void (*console_cmd_handler_t)(rayc_t * rayc, int argc, char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]);

typedef struct {
  const char *          name;
  const char *          help;
  console_cmd_handler_t fn;
} console_cmd_t;

typedef struct {
  char    line[CONSOLE_LINE_MAX];
  color_t color;
} console_line_t;

typedef struct {
  bool enabled;

  struct {
    console_line_t buf[CONSOLE_LINE_BUF_SIZE];
    int head, tail;
  } lines;
} console_t;

void rayc_console_puts(rayc_t * rayc, color_t color, const char * buf);
void rayc_console_printf(rayc_t * rayc, color_t color, const char * fmt, ...);

void rayc_draw_console(rayc_t * rayc);

void rayc_console_enter(rayc_t * rayc);
void rayc_console_exit(rayc_t * rayc);

const console_cmd_t * rayc_console_get_cmd(const char * name);
