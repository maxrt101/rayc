#ifndef _RAYC_CORE_APP_H
#define _RAYC_CORE_APP_H

#include <queue>
#include <vector>
#include <string>
#include <functional>

#include <SDL2/SDL.h>

#include "io/input.h"
#include "core/store.h"
#include "core/context.h"
#include "core/component.h"
#include "utils/config.h"

namespace rayc {

class Application {
 private:
  struct FrameKeyState {
    bool pressed = false;
    bool released = false;
  };

  // static constexpr size_t NumberOfKeys = 322;

 private:
  bool m_isRunning = false;
  bool m_isInitialized = false;

  Config m_config;
  Store m_store;

  int m_cycles = 0;
  int m_fpsCap = 100;

  // Keys
  FrameKeyState m_keys[322];
  bool m_held_keys[322];
  std::string m_textInputLine;
  bool m_textInputEnabled = false;

  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;

  // Components
  // static std::function<bool(Component*, Component*)> comparator;
  // std::priority_queue<Component*, std::vector<Component*>, decltype(comparator)> m_components;
  std::vector<Component*> m_components;

 public:
  Application(/*const std::string& appName,*/ Config config);
  ~Application();

  Context* createContext();

  void run();
  void stop();

  void attachComponent(Component* component);
  void detachComponent(const std::string& name);

  Config& getConfig();
  Store& getStore();

  int getWidth() const;
  int getHeight() const;
  
  void setFpsCap(int cap);
  int getFpsCap() const;

  KeyState getKeyState(SDL_Scancode sc) const;
  void enableTextInput();
  void disableTextInput();

  SDL_Window* getWindow() const;
  SDL_Renderer* getRenderer();

 private:
  void cleanup();
  void resetKeys();
  void clearScreen();
  void updateScreen();
  void reorderComponents();
  void updateKeydown(SDL_Scancode sc);
};

class ApplicationContext : public Context {
 private:
  Application* m_app = nullptr;

 public:
  ApplicationContext(Application* app);
  ~ApplicationContext() = default;

  int getWidth() const override;
  int getHeight() const override;

  int getX() const override;
  int getY() const override;

  SDL_Window* getWindow() const override;
  SDL_Renderer* getRenderer() const override;

  KeyState getKeyState(SDL_Scancode sc) const override;

  Config& getConfig() override;
  Store& getStore() override;
};

} /* namespace rayc */

#endif