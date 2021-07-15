#ifndef _RAYC_CORE_APP_H
#define _RAYC_CORE_APP_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "core/input.h"
#include "core/layer.h"

namespace rayc {

class Application {
 private:
  struct FrameKeyState {
    bool pressed = false;
    bool released = false;
  };

 public:
  Application(const std::string& name, int width, int height);
  ~Application();

  void run();
  void stop();

  void pushLayer(Layer* layer);
  Layer* popLayer();
  Layer* getLayer(const std::string& name);

  int getWidth() const;
  int getHeight() const;

  void setFpsCap(int cap);
  int getFpsCap() const;
 
  KeyState getKeyState(SDL_Scancode sc) const;
  void enableTextInput();
  void disableTextInput();

  Layer* getCurrentLayer() const;
  SDL_Window* getWindow() const;
  static SDL_Renderer* getRenderer();

 private:
  void cleanup();
  void resetKeys();
  void clearScreen();
  void updateScreen();

  void updateKeydown(SDL_Scancode sc);
  void drawLayer(Layer* layer);

 private:
  bool m_running = false;
  bool m_initialized = false;

  std::string m_app_name;
  int m_screen_width;
  int m_screen_height;

  int m_cycles = 0;
  float m_fps_cap = 100;

  FrameKeyState m_keys[322];
  bool m_held_keys[322];
  std::string m_text_input_line;
  bool m_text_input_enabled = false;

  SDL_Window* m_window = nullptr;

  std::vector<Layer*> m_layers; // or ref, or unique_ptr
  Layer* m_current_layer = nullptr;

  static SDL_Renderer* m_renderer;
};

}; /* namespace rayc */

#endif