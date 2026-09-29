#include "rayc/rayc.h"
#include "rayc/gfx.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define X(__name, __help, __fn) void __fn(rayc_t *, int, char [CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX]);
CONSOLE_COMMANDS
#undef X

static const console_cmd_t console_commands[] = {
#define X(name, desc, func) { #name, desc, func },
CONSOLE_COMMANDS
#undef X
};

static void console_line_cb(void * ctx, const char * buf) {
  rayc_t * rayc = ctx;

  rayc_console_puts(rayc, COLOR_YELLOW, buf);

  char argv[CONSOLE_ARGV_MAX][CONSOLE_ARG_MAX] = {0};
  int argc = 0;
  int i = 0;

  while (buf[i] != '\0' && argc < CONSOLE_ARGV_MAX) {
    // Skip leading whitespace
    while (buf[i] == ' ' || buf[i] == '\t') {
      i++;
    }

    if (buf[i] == '\0') break;

    // Extract the word
    int arg_len = 0;
    while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\0') {
      // Only copy if we haven't hit the max argument length
      if (arg_len < CONSOLE_ARG_MAX - 1) {
        argv[argc][arg_len++] = buf[i];
      }
      i++;
    }

    // Null-terminate the current argument and increment argc
    argv[argc][arg_len] = '\0';
    argc++;
  }

  // execute
  if (argc > 0) {
    const console_cmd_t * cmd = rayc_console_get_cmd(argv[0]);

    if (cmd) {
      cmd->fn(rayc, argc, argv);
    }
  }
}

static void draw_line_buf(const rayc_t * rayc, const int start_x, const int start_y) {
  const int fh = font_get_height(rayc->font);

  int current_idx = rayc->console.lines.tail;
  int line_num = 0;

  while (current_idx != rayc->console.lines.head) {
    const int y_pos = start_y + line_num * fh;

    gfx_draw_string(rayc->font, start_x, y_pos, rayc->console.lines.buf[current_idx].color, rayc->console.lines.buf[current_idx].line);

    current_idx = (current_idx + 1) % CONSOLE_LINE_BUF_SIZE;
    line_num++;
  }
}

void rayc_draw_console(rayc_t * rayc) {
  const int fw = font_get_width(rayc->font);
  const int fh = font_get_height(rayc->font);

  int y_max = HEIGHT / 100.0f * 70;

  y_max = y_max - y_max % fh;

  gfx_fill_rect(0, 0, WIDTH, y_max, RGBA(30, 30, 30, 200));

  draw_line_buf(rayc, 0, 0);

  gfx_draw_string(rayc->font, 0, y_max - fh, COLOR_YELLOW, CONSOLE_PROMPT);
  gfx_draw_string(rayc->font, (int)(fw * (sizeof(CONSOLE_PROMPT) - 1)), y_max - fh, COLOR_WHITE, rayc->input.line.buf);
}

void rayc_console_puts(rayc_t * rayc, color_t color, const char * buf) {
  ASSERT_RET(rayc && buf);

  memcpy(rayc->console.lines.buf[rayc->console.lines.head].line, buf, strlen(buf)+1);
  rayc->console.lines.buf[rayc->console.lines.head].color = color;

  rayc->console.lines.head = (rayc->console.lines.head + 1) % CONSOLE_LINE_BUF_SIZE;

  if (rayc->console.lines.head == rayc->console.lines.tail) {
    rayc->console.lines.tail = (rayc->console.lines.tail + 1) % CONSOLE_LINE_BUF_SIZE;
  }
}

void rayc_console_printf(rayc_t * rayc, color_t color, const char * fmt, ...) {
  char buf[CONSOLE_LINE_MAX];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  rayc_console_puts(rayc, color, buf);
}

const console_cmd_t * rayc_console_get_cmd(const char * name) {
  for (int i = 0; i < ARR_COUNT(console_commands); ++i) {
    if (!strcmp(console_commands[i].name, name)) {
      return &console_commands[i];
    }
  }

  return NULL;
}

void rayc_console_enter(rayc_t * rayc) {
  rayc->input.mode = INPUT_MODE_LINE;
  input_set_line_cb(&rayc->input, console_line_cb, rayc);
}

void rayc_console_exit(rayc_t * rayc) {
  rayc->input.mode = INPUT_MODE_DEFAULT;
  input_set_line_cb(&rayc->input, NULL, NULL);
}
