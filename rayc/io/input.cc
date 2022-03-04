#include "io/input.h"

rayc::KeyState::KeyState(bool p, bool h, bool r)
  : pressed(p), held(h), released(r) {}
