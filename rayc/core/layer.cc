#include "core/layer.h"
#include "core/app.h"

rayc::Layer::Layer(Application* app, const std::string& name, int width, int height, int x, int y)
    : m_app(app), m_name(name), m_width(width), m_height(height), m_x(x), m_y(y) {
  m_buffer = SDL_CreateTexture(Application::getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(m_buffer, SDL_BLENDMODE_BLEND);
}

rayc::Layer::~Layer() {
  SDL_DestroyTexture(m_buffer);
}

void rayc::Layer::move(int x, int y) {
  m_x = x;
  m_y = y;
}

// void rayc::Layer::resize(int w, int h) {}

void rayc::Layer::hide() {
  m_is_hidden = true;
  onHide();
}

void rayc::Layer::show() {
  m_is_hidden = false;
  onShow();
}

void rayc::Layer::close() {
  m_will_close = true;
}

rayc::Application* rayc::Layer::getApp() const {
  return m_app;
}

SDL_Renderer* rayc::Layer::getRenderer() const {
  return Application::getRenderer();
}

void rayc::Layer::clearBuffer() const {
  SDL_SetRenderTarget(getRenderer(), m_buffer);
  SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 0);
  SDL_RenderClear(getRenderer());
}

std::string rayc::Layer::getName() const {
  return m_name;
}

int rayc::Layer::getWidth() const {
  return m_width;
}

int rayc::Layer::getHeight() const {
  return m_height;
}

int rayc::Layer::getX() const{
  return m_x;
}

int rayc::Layer::getY() const {
  return m_y;
}

void rayc::Layer::onPush() {}

void rayc::Layer::onShow() {}

void rayc::Layer::onHide() {}
