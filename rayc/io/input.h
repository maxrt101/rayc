#ifndef _RAYC_IO_INPUT_H
#define _RAYC_IO_INPUT_H

#include <string>

namespace rayc {

struct KeyState {
  bool pressed = false;
  bool held = false;
  bool released = false;

  KeyState() = default;
  KeyState(bool p, bool h, bool r);
};

struct TextInputListener {
  virtual void onTextInputReady(const std::string& input) = 0;
  virtual void onTextInputEvent(const std::string& input) = 0;
};

} /* namespace rayc */

#endif