#include "app.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "core/components/drawable.h"
#include "core/components/functional.h"
#include "utils/log.h"
#include "utils/die.h"
#include "version.h"

rayc::Application::Application(/*const std::string& appName,*/ Config config)
    : m_config(config) /*m_appName(appName), m_screenWidth(width), m_screenHeight(height)*/ {
  printf("rayc v%s\n", RAYC_VERSION_STRING);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    sdlFatal("SDL_Init failed");
    die();
  }

  if (IMG_Init(IMG_INIT_JPG) < 0) {
    sdlFatal("IMG_Init failed");
    die();
  }

  if (TTF_Init() < 0) {
    sdlFatal("TTF_Init failed");
    die();
  }

  m_window = SDL_CreateWindow(
    // m_appName.c_str(),
    m_config.getValueOr("window", "name", "test").c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    getWidth(),
    getHeight(),
    SDL_WINDOW_SHOWN
  );

  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  if (m_renderer == NULL) {
    sdlFatal("Renderer creation failed");
    die();
  }

  SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_BLEND);

  memset(&m_keys, 0, 322*sizeof(FrameKeyState));
  memset(&m_held_keys, 0, 322);

  info("rayc initialized succesfully");
  m_isInitialized = true;
}

rayc::Application::~Application() {
  stop();
  if (m_isInitialized) {
    cleanup();
  }
}

rayc::Context* rayc::Application::createContext() {
  return new ApplicationContext(this);
}

void rayc::Application::run() {
  auto time1 = std::chrono::system_clock::now();
  auto time2 = std::chrono::system_clock::now();
  auto time3 = std::chrono::system_clock::now();

  float frameTime = 0.1;        // Time spent rendering
  float actualFrameTime = 0.1;  // Time spent in this function during 1 frame

  SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(getRenderer());

  m_isRunning = true;

  while (m_isRunning) {
    time1 = std::chrono::system_clock::now();

    m_cycles++;
    float targetFrameTime = 1.0f/m_fpsCap;

    memset(&m_keys, 0, 322*sizeof(FrameKeyState));

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          stop();
          break;
        }
        case SDL_KEYDOWN: {
          SDL_Scancode sc = event.key.keysym.scancode;
          if (m_textInputEnabled) {
            switch (sc) {
              case SDL_SCANCODE_BACKSPACE: {
                if (m_textInputLine.size() > 0) {
                  m_textInputLine.pop_back();
                  for (Component* component : m_components) {
                    TextInputListener* listener = dynamic_cast<TextInputListener*>(component);
                    if (listener) {
                      listener->onTextInputEvent("\b");
                    }
                  }
                }
                break;
              }
              case SDL_SCANCODE_RETURN: {
                if (m_textInputLine.size() > 0) {
                  for (Component* component : m_components) {
                    TextInputListener* listener = dynamic_cast<TextInputListener*>(component);
                    if (listener) {
                      listener->onTextInputReady(m_textInputLine);
                    }
                  }
                  m_textInputLine.clear();
                }
                break;
              }
              case SDL_SCANCODE_ESCAPE:
              case SDL_SCANCODE_GRAVE: {
                updateKeydown(sc);
                break;
              }
              default: {
                break;
              }
            }
          } else {
            updateKeydown(sc);
          }
          break;
        }
        case SDL_KEYUP: {
          SDL_Scancode sc = event.key.keysym.scancode;
          m_keys[sc].released = true;
           m_held_keys[sc] = false;
          break;
        }
        case SDL_TEXTINPUT: {
          m_textInputLine += event.text.text;
          for (Component* component : m_components) {
            TextInputListener* listener = dynamic_cast<TextInputListener*>(component);
            if (listener) {
              listener->onTextInputEvent(event.text.text);
            }
          }
          break;
        }
        default: {
          break;
        } 
      }
    }

    if (!m_isRunning) {
      break;
    }

    SDL_SetRenderTarget(getRenderer(), NULL);
    clearScreen();

    /*for (auto component : m_components) {
      if (component->isAttached() && component->getPriority() > 0) {
        if (!component->isHidden()) {
           component->onDraw();
        }
        if (component->isActive()) {
          component->onFrameUpdate(frameTime);
        }
      }
    }*/

    std::for_each(m_components.begin(), m_components.end(), std::bind(&Component::update, std::placeholders::_1, frameTime));

    /*std::remove_if(m_components.begin(), m_components.end(),
      [](auto component) { return !component->isAttached(); });*/

    std::remove_if(m_components.begin(), m_components.end(), std::bind(&Component::isAttached, std::placeholders::_1));

    updateScreen();

    time2 = std::chrono::system_clock::now();
    std::chrono::duration<float> frameDuration = time2 - time1;
    frameTime = frameDuration.count();

    // Fps cap
    if (frameTime < targetFrameTime) {
      SDL_Delay((targetFrameTime - frameTime) * 1000);
    }

    time3 = std::chrono::system_clock::now();
    std::chrono::duration<float> actualFrameDuration = time3 - time1;
    actualFrameTime = actualFrameDuration.count();
  }
}

void rayc::Application::stop() {
  m_isRunning = false;
}

void rayc::Application::attachComponent(Component* component) {
  if (!component) {
    rayc::error("Trying to attach null component");
    return;
  }
  m_components.push_back(component);
  reorderComponents();
}

void rayc::Application::detachComponent(const std::string& name) {
  std::remove_if(m_components.begin(), m_components.end(), [&name](Component* component) {
    return component->getName() == name;
  });
}

rayc::Config& rayc::Application::getConfig() {
  return m_config;
}

int rayc::Application::getWidth() const {
  return std::stoi(m_config.getValueOr("window", "width", "800"));
}

int rayc::Application::getHeight() const {
  return std::stoi(m_config.getValueOr("window", "height", "600"));
}

void rayc::Application::setFpsCap(int cap) {
  m_fpsCap = cap;
}

int rayc::Application::getFpsCap() const {
  return m_fpsCap;
}

rayc::KeyState rayc::Application::getKeyState(SDL_Scancode sc) const {
  return KeyState(m_keys[sc].pressed, m_held_keys[sc], m_keys[sc].released);
}

void rayc::Application::enableTextInput() {
  SDL_StartTextInput();
}

void rayc::Application::disableTextInput() {
  SDL_StopTextInput();
}

SDL_Window* rayc::Application::getWindow() const {
  return m_window;
}

SDL_Renderer* rayc::Application::getRenderer() {
  return m_renderer;
}

void rayc::Application::cleanup() {
  TTF_Quit();
  IMG_Quit();
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();

  info("rayc stopped");
}

void rayc::Application::resetKeys() {
  for (int i = 0; i < 322; i++) {
    m_keys[i].pressed = false;
    m_keys[i].released = false;
    m_held_keys[i] = false;
  }
}

void rayc::Application::clearScreen() {
  SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(getRenderer());
}

void rayc::Application::updateScreen() {
  SDL_RenderPresent(getRenderer());
}

void rayc::Application::reorderComponents() {
  std::sort(m_components.begin(), m_components.end(), [](rayc::Component* lhs, rayc::Component* rhs) {
    return lhs->getPriority() < rhs->getPriority();
  });
}

void rayc::Application::updateKeydown(SDL_Scancode sc) {
  if (!m_held_keys[sc]) {
    m_keys[sc].pressed = true;
    m_held_keys[sc] = true;
  }
}

// Application Context

rayc::ApplicationContext::ApplicationContext(Application* app) : m_app(app) {}

int rayc::ApplicationContext::getWidth() const {
  return m_app->getWidth();
}

int rayc::ApplicationContext::getHeight() const {
  return m_app->getHeight();
}

int rayc::ApplicationContext::getX() const {
  return 0;
}

int rayc::ApplicationContext::getY() const {
  return 0;
}

SDL_Window* rayc::ApplicationContext::getWindow() const {
  return m_app->getWindow();
}

SDL_Renderer* rayc::ApplicationContext::getRenderer() const {
  return m_app->getRenderer();
}

rayc::KeyState rayc::ApplicationContext::getKeyState(SDL_Scancode sc) const {
  return m_app->getKeyState(sc);
}

rayc::Config& rayc::ApplicationContext::getConfig() {
  return m_app->getConfig();
}

rayc::Store& rayc::ApplicationContext::getStore() {
  return m_app->getStore();
}
