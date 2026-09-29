#pragma once

typedef enum {
  SCANCODE_UNKNOWN = 0,

  SCANCODE_A,
  SCANCODE_B,
  SCANCODE_C,
  SCANCODE_D,
  SCANCODE_E,
  SCANCODE_F,
  SCANCODE_G,
  SCANCODE_H,
  SCANCODE_I,
  SCANCODE_J,
  SCANCODE_K,
  SCANCODE_L,
  SCANCODE_M,
  SCANCODE_N,
  SCANCODE_O,
  SCANCODE_P,
  SCANCODE_Q,
  SCANCODE_R,
  SCANCODE_S,
  SCANCODE_T,
  SCANCODE_U,
  SCANCODE_V,
  SCANCODE_W,
  SCANCODE_X,
  SCANCODE_Y,
  SCANCODE_Z,

  SCANCODE_1,
  SCANCODE_2,
  SCANCODE_3,
  SCANCODE_4,
  SCANCODE_5,
  SCANCODE_6,
  SCANCODE_7,
  SCANCODE_8,
  SCANCODE_9,
  SCANCODE_0,

  SCANCODE_RETURN,
  SCANCODE_ESCAPE,
  SCANCODE_BACKSPACE,
  SCANCODE_TAB,
  SCANCODE_SPACE,

  SCANCODE_MINUS,
  SCANCODE_EQUALS,
  SCANCODE_LEFTBRACKET,
  SCANCODE_RIGHTBRACKET,
  SCANCODE_SEMICOLON,
  SCANCODE_APOSTROPHE,
  SCANCODE_GRAVE,
  SCANCODE_COMMA,
  SCANCODE_PERIOD,
  SCANCODE_SLASH,

  // TODO: Backlash/vertical line

  SCANCODE_F1,
  SCANCODE_F2,
  SCANCODE_F3,
  SCANCODE_F4,
  SCANCODE_F5,
  SCANCODE_F6,
  SCANCODE_F7,
  SCANCODE_F8,
  SCANCODE_F9,
  SCANCODE_F10,
  SCANCODE_F11,
  SCANCODE_F12,

  SCANCODE_RIGHT,
  SCANCODE_LEFT,
  SCANCODE_DOWN,
  SCANCODE_UP,

  SCANCODE_LCTRL,
  SCANCODE_LSHIFT,
  SCANCODE_LALT,
  SCANCODE_LMETA,
  SCANCODE_RCTRL,
  SCANCODE_RSHIFT,
  SCANCODE_RALT,
  SCANCODE_RMETA,

  __SCANCODE_MAX,
} scancode_t;

typedef enum {
  INPUT_MODE_DEFAULT = 0,
  INPUT_MODE_LINE,
} input_mode_t;

typedef struct {
  bool pressed;
  bool released;
  bool held;
} key_state_t;

typedef void (*input_line_cb_t)(void * ctx, const char * buf);

typedef struct {
  key_state_t kb[__SCANCODE_MAX];

  struct {
    int x, y;

    bool left_btn;
    bool right_btn;
  } mouse;

  struct {
    char buf[32];
    int  index;

    struct {
      input_line_cb_t fn;
      void * ctx;
    } cb;
  } line;

  input_mode_t mode;
} input_t;

char scancode2char(scancode_t sc);
char scancode2char_shifted(scancode_t sc);

void input_set_line_cb(input_t * input, input_line_cb_t fn, void * ctx);

void input_reset_press_release(input_t * input);
void input_key_down(input_t * input, scancode_t sc);
void input_key_up(input_t * input, scancode_t sc);
