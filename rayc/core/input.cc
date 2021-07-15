#include "core/input.h"

rayc::KeyState::KeyState() {}

rayc::KeyState::KeyState(bool p, bool h, bool r)
  : pressed(p), held(h), released(r) {}
