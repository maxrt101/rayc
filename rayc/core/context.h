#ifndef _RAYC_CORE_CONTEXT_H
#define _RAYC_CORE_CONTEXT_H

#include <SDL2/SDL.h>

#include "io/input.h"
#include "core/store.h"
#include "utils/config.h"

namespace rayc {

class Context {
 public:
  Context() = default;
  virtual ~Context() = default;

  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  virtual int getX() const = 0;
  virtual int getY() const = 0;

  virtual SDL_Window* getWindow() const = 0;
  virtual SDL_Renderer* getRenderer() const = 0;

  virtual KeyState getKeyState(SDL_Scancode sc) const = 0;

  virtual Config& getConfig() = 0;
  virtual Store& getStore() = 0;
  // virtual 
};

} /* namespace rayc */

#endif