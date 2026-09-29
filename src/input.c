#include "rayc/input.h"

char scancode2char(const scancode_t sc) {
  switch (sc) {
    case SCANCODE_A:            return 'a';
    case SCANCODE_B:            return 'b';
    case SCANCODE_C:            return 'c';
    case SCANCODE_D:            return 'd';
    case SCANCODE_E:            return 'e';
    case SCANCODE_F:            return 'f';
    case SCANCODE_G:            return 'g';
    case SCANCODE_H:            return 'h';
    case SCANCODE_I:            return 'i';
    case SCANCODE_J:            return 'j';
    case SCANCODE_K:            return 'k';
    case SCANCODE_L:            return 'l';
    case SCANCODE_M:            return 'm';
    case SCANCODE_N:            return 'n';
    case SCANCODE_O:            return 'o';
    case SCANCODE_P:            return 'p';
    case SCANCODE_Q:            return 'q';
    case SCANCODE_R:            return 'r';
    case SCANCODE_S:            return 's';
    case SCANCODE_T:            return 't';
    case SCANCODE_U:            return 'u';
    case SCANCODE_V:            return 'v';
    case SCANCODE_W:            return 'w';
    case SCANCODE_X:            return 'x';
    case SCANCODE_Y:            return 'y';
    case SCANCODE_Z:            return 'z';
    case SCANCODE_1:            return '1';
    case SCANCODE_2:            return '2';
    case SCANCODE_3:            return '3';
    case SCANCODE_4:            return '4';
    case SCANCODE_5:            return '5';
    case SCANCODE_6:            return '6';
    case SCANCODE_7:            return '7';
    case SCANCODE_8:            return '8';
    case SCANCODE_9:            return '9';
    case SCANCODE_0:            return '0';
    case SCANCODE_RETURN:       return '\n';
    // case SCANCODE_ESCAPE:       return 'ESCAPE';
    case SCANCODE_BACKSPACE:    return '\b';
    case SCANCODE_TAB:          return '\t';
    case SCANCODE_SPACE:        return ' ';
    case SCANCODE_MINUS:        return '-';
    case SCANCODE_EQUALS:       return '=';
    case SCANCODE_LEFTBRACKET:  return '[';
    case SCANCODE_RIGHTBRACKET: return ']';
    case SCANCODE_SEMICOLON:    return ';';
    case SCANCODE_APOSTROPHE:   return '\'';
    case SCANCODE_GRAVE:        return '`';
    case SCANCODE_COMMA:        return ',';
    case SCANCODE_PERIOD:       return '.';
    case SCANCODE_SLASH:        return '/';
    default:                    return '\0';
  }
}

char scancode2char_shifted(const scancode_t sc) {
  switch (sc) {
    case SCANCODE_A:            return 'A';
    case SCANCODE_B:            return 'B';
    case SCANCODE_C:            return 'C';
    case SCANCODE_D:            return 'D';
    case SCANCODE_E:            return 'E';
    case SCANCODE_F:            return 'F';
    case SCANCODE_G:            return 'G';
    case SCANCODE_H:            return 'H';
    case SCANCODE_I:            return 'I';
    case SCANCODE_J:            return 'J';
    case SCANCODE_K:            return 'K';
    case SCANCODE_L:            return 'L';
    case SCANCODE_M:            return 'M';
    case SCANCODE_N:            return 'N';
    case SCANCODE_O:            return 'O';
    case SCANCODE_P:            return 'P';
    case SCANCODE_Q:            return 'Q';
    case SCANCODE_R:            return 'R';
    case SCANCODE_S:            return 'S';
    case SCANCODE_T:            return 'T';
    case SCANCODE_U:            return 'U';
    case SCANCODE_V:            return 'V';
    case SCANCODE_W:            return 'W';
    case SCANCODE_X:            return 'X';
    case SCANCODE_Y:            return 'Y';
    case SCANCODE_Z:            return 'Z';
    case SCANCODE_1:            return '!';
    case SCANCODE_2:            return '@';
    case SCANCODE_3:            return '#';
    case SCANCODE_4:            return '$';
    case SCANCODE_5:            return '%';
    case SCANCODE_6:            return '^';
    case SCANCODE_7:            return '&';
    case SCANCODE_8:            return '*';
    case SCANCODE_9:            return '(';
    case SCANCODE_0:            return ')';
    case SCANCODE_RETURN:       return '\n';
    // case SCANCODE_ESCAPE:       return 'ESCAPE';
    case SCANCODE_BACKSPACE:    return '\b';
    case SCANCODE_TAB:          return '\t';
    case SCANCODE_SPACE:        return ' ';
    case SCANCODE_MINUS:        return '_';
    case SCANCODE_EQUALS:       return '+';
    case SCANCODE_LEFTBRACKET:  return '{';
    case SCANCODE_RIGHTBRACKET: return '}';
    case SCANCODE_SEMICOLON:    return ':';
    case SCANCODE_APOSTROPHE:   return '"';
    case SCANCODE_GRAVE:        return '~';
    case SCANCODE_COMMA:        return '<';
    case SCANCODE_PERIOD:       return '>';
    case SCANCODE_SLASH:        return '?';
    default:                    return '\0';
  }
}

static void line_mode_key_down(input_t * input, scancode_t sc) {
  if (sc == SCANCODE_BACKSPACE) {
    if (input->line.index > 0) {
      input->line.index--;
      input->line.buf[input->line.index] = '\0';
    }
    return;
  }

  if (sc == SCANCODE_RETURN) {
    if (input->line.cb.fn) {
      input->line.cb.fn(input->line.cb.ctx, input->line.buf);
    }
    input->line.index = 0;
    input->line.buf[0] = '\0';
    return;
  }

  const char c = input->kb[SCANCODE_LSHIFT].held ? scancode2char_shifted(sc) : scancode2char(sc);

  if (c) {
    input->line.buf[input->line.index++] = c;
    input->line.buf[input->line.index] = '\0';
  }
}

void input_set_line_cb(input_t * input, input_line_cb_t fn, void * ctx) {
  input->line.cb.fn = fn;
  input->line.cb.ctx = ctx;
}

void input_reset_press_release(input_t * input) {
  for (scancode_t sc = 0; sc < __SCANCODE_MAX; ++sc) {
    input->kb[sc].pressed = false;
    input->kb[sc].released = false;
  }
}

void input_key_down(input_t * input, scancode_t sc) {
  if (!input->kb[sc].held) {
    input->kb[sc].pressed = true;
    input->kb[sc].held = true;
  } else {
    input->kb[sc].pressed = false;
  }

  if (input->mode == INPUT_MODE_LINE) {
    // FIXME: Dirty hack to stop '`' from appearing in input line
    if (sc != SCANCODE_GRAVE) {
      line_mode_key_down(input, sc);
    }
  }
}

void input_key_up(input_t * input, scancode_t sc) {
  input->kb[sc].pressed = false;
  input->kb[sc].held = false;
  input->kb[sc].released = true;
}
