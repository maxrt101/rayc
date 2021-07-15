#ifndef _RAYC_CORE_KEYS_H
#define _RAYC_CORE_KEYS_H

#include <string>

#include <SDL2/SDL.h>

#include "event.h"

namespace rayc {

struct KeyState {
 bool pressed = false;
 bool held = false;
 bool released = false;

 KeyState();
 KeyState(bool p, bool h, bool r);
};

struct TextInputListener {
  virtual void onTextInputReady(const std::string& input) = 0;
  virtual void onTextInputEvent(const std::string& input) = 0;
};

} /* namespace rayc */

#endif