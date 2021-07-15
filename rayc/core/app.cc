#include "core/app.h"

#include <chrono>
#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "utils/log.h"
#include "utils/version.h"

SDL_Renderer* rayc::Application::m_renderer = nullptr;

rayc::Application::Application(const std::string& name, int width, int height)
    : m_app_name(name), m_screen_width(width), m_screen_height(height) {
  std::cout << "rayc v" << RAYC_VERSION_STRING << "\n";

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    sdlError("SDL_Init failed");
    exit(EXIT_FAILURE);
  }

  if (IMG_Init(IMG_INIT_JPG) < 0) {
    sdlError("IMG_Init failed");
    exit(EXIT_FAILURE);
  }

  if (TTF_Init() < 0) {
    sdlError("TTF_Init failed");
    exit(EXIT_FAILURE);
  }

  m_window = SDL_CreateWindow(
    m_app_name.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    m_screen_width,
    m_screen_height,
    SDL_WINDOW_SHOWN
  );

  if (m_window == NULL) {
    sdlError("Window creation failed");
    exit(EXIT_FAILURE);
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  if (m_renderer == NULL) {
    sdlError("Renderer creation failed");
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_BLEND);

  memset(&m_keys, 0, 322*sizeof(FrameKeyState));
  memset(&m_held_keys, 0, 322);
  
  info("rayc initialized.");
  m_initialized = true;
}

rayc::Application::~Application() {
  stop();
  if (m_initialized) {
    cleanup();
  }
}

void rayc::Application::cleanup() {
  TTF_Quit();
  IMG_Quit();
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();

  info("rayc stopped");
}

void rayc::Application::run() {
  auto time1 = std::chrono::system_clock::now();
  auto time2 = std::chrono::system_clock::now();
  auto time3 = std::chrono::system_clock::now();

  float frame_time = 0.1;         // Time spent rendering
  float actual_frame_time = 0.1;  // Time spent in this function during 1 frame

  SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(getRenderer());

  m_running = true;

  while (m_running) {
    time1 = std::chrono::system_clock::now();

    m_cycles++;
    float target_frame_time = 1.0f/m_fps_cap;

    memset(&m_keys, 0, 322*sizeof(FrameKeyState));

    TextInputListener* input_listener = dynamic_cast<TextInputListener*>(m_current_layer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        stop();
      } else if (event.type == SDL_KEYDOWN) {
        SDL_Scancode sc = event.key.keysym.scancode;
        if (m_text_input_enabled) {
          if (sc == SDL_SCANCODE_BACKSPACE) {
            if (m_text_input_line.size() > 0) {
              m_text_input_line.pop_back();
              if (input_listener) {
                input_listener->onTextInputEvent("\b");
              }
            }
          } else if (sc == SDL_SCANCODE_RETURN) {
            if (m_text_input_line.size() > 0) {
              if (input_listener) {
                input_listener->onTextInputReady(m_text_input_line);
                m_text_input_line = "";
              }
            }
          } else if (sc == SDL_SCANCODE_ESCAPE || sc == SDL_SCANCODE_GRAVE) {
            updateKeydown(sc);
          }
        } else {
          updateKeydown(sc);
        }
      } else if (event.type == SDL_KEYUP) {
        SDL_Scancode sc = event.key.keysym.scancode;
        m_keys[sc].released = true;
        m_held_keys[sc] = false;
      } else if (event.type == SDL_TEXTINPUT) {
        m_text_input_line += event.text.text;
        if (input_listener) {
          input_listener->onTextInputEvent(event.text.text);
        }
      }
    }

    if (!m_running) {
      break;
    }

    SDL_SetRenderTarget(getRenderer(), NULL);
    clearScreen();

    if (m_current_layer) {
      m_current_layer->onFrameUpdate(actual_frame_time);
    }
  
    auto old_current = m_current_layer; // debug

    for (Layer* layer : m_layers) {
      if (!layer->m_is_hidden) {
        drawLayer(layer);

        m_current_layer = layer;
        
        for (Layer* sub_layer : layer->m_sublayers) {
          if (!sub_layer->m_is_hidden) {
            drawLayer(sub_layer);
            m_current_layer = sub_layer;
          }
        }
      }
    }

    std::remove_if(m_layers.begin(), m_layers.end(),
      [](auto layer) { return layer->m_will_close; });

    if (old_current != m_current_layer) {
      debug("current layer changed: %s@%p -> %s@%p",
        old_current->getName().c_str(), old_current,
        m_current_layer->getName().c_str(), m_current_layer);
    }

    updateScreen();

    time2 = std::chrono::system_clock::now();
    std::chrono::duration<float> frame_duration = time2 - time1;
    frame_time = frame_duration.count();

    // Fps cap
    if (frame_time < target_frame_time) {
      SDL_Delay((target_frame_time - frame_time) * 1000);
    }

    time3 = std::chrono::system_clock::now();
    std::chrono::duration<float> actual_frame_duration = time3 - time1;
    actual_frame_time = actual_frame_duration.count();
  }
}

void rayc::Application::stop() {
  m_running = false;
}

void rayc::Application::pushLayer(Layer* layer) {
  if (!layer->m_is_hidden) {
    // if (m_current_layer) {
      // m_current_layer->m_is_top_layer = false;
    // }
    m_current_layer = layer;
    // m_current_layer->m_is_top_layer = true;
  }
  m_layers.push_back(layer);
  layer->onPush();
}

rayc::Layer* rayc::Application::popLayer() {
  Layer* layer = m_layers.back();
  // layer->m_is_top_layer = false;
  m_layers.pop_back();
  if (m_current_layer == layer) {
    m_current_layer = m_layers.back();
  // m_current_layer->m_is_top_layer = true;
  }
  return layer;
}

rayc::Layer* rayc::Application::getLayer(const std::string& name) {
  for (auto& layer : m_layers) {
    if (layer->getName() == name) {
      return layer;
    }
  }
  return nullptr;
}

int rayc::Application::getWidth() const {
  return m_screen_width;
}

int rayc::Application::getHeight() const {
  return m_screen_height;
}

void rayc::Application::setFpsCap(int cap) {
  m_fps_cap = cap;
}

int rayc::Application::getFpsCap() const {
  return m_fps_cap;
}

void rayc::Application::enableTextInput() {
  m_text_input_enabled = true;
  SDL_StartTextInput();
  m_text_input_line.clear();
}

void rayc::Application::disableTextInput() {
  m_text_input_enabled = false;
  SDL_StopTextInput();
}

rayc::Layer* rayc::Application::getCurrentLayer() const {
  return m_current_layer;
}

SDL_Renderer* rayc::Application::getRenderer() {
  return m_renderer;
}

void rayc::Application::resetKeys() {
  for (int i = 0; i < 322; i++) {
    m_keys[i].pressed = false;
    m_keys[i].released = false;
    m_held_keys[i] = false;
  }
}

rayc::KeyState rayc::Application::getKeyState(SDL_Scancode sc) const {
  return KeyState(m_keys[sc].pressed, m_held_keys[sc], m_keys[sc].released);
}

void rayc::Application::updateKeydown(SDL_Scancode sc) {
  if (!m_held_keys[sc]) {
    m_keys[sc].pressed = true;
    m_held_keys[sc] = true;
  }
}

void rayc::Application::drawLayer(Layer* layer) {
  SDL_SetRenderTarget(getRenderer(), layer->m_buffer);

  if (layer->onDraw()) {
    SDL_Rect dest;
    dest.x = layer->m_x;
    dest.y = layer->m_y;
    dest.w = layer->m_width;
    dest.h = layer->m_height;

    SDL_SetRenderTarget(getRenderer(), NULL);
    SDL_RenderCopy(getRenderer(), layer->m_buffer, NULL, &dest);
  }
}

SDL_Window* rayc::Application::getWindow() const {
  return m_window;
}

void rayc::Application::clearScreen() {
  SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(getRenderer());
}

void rayc::Application::updateScreen() {
  SDL_RenderPresent(getRenderer());
}