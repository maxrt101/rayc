#include "core/components/drawable.h"

rayc::DrawableComponent::DrawableComponent(Context* ctx, int priority, const std::string& name, int width, int height, int x, int y)
  : Component(ComponentType::DRAWABLE, priority, ctx, name) {}

rayc::DrawableComponent::~DrawableComponent() {}

void rayc::DrawableComponent::draw() const {
  SDL_SetRenderTarget(getContext()->getRenderer(), m_buffer);

  SDL_Rect dest;
  dest.x = m_xOffset;
  dest.y = m_yOffset;
  dest.w = m_width;
  dest.h = m_height;

  SDL_SetRenderTarget(getContext()->getRenderer(), NULL);
  SDL_RenderCopy(getContext()->getRenderer(), m_buffer, NULL, &dest);
}

void rayc::DrawableComponent::move(int x, int y) {
  m_xOffset = x;
  m_yOffset = y;
}

void rayc::DrawableComponent::show() {
  m_isHidden = false;
}

void rayc::DrawableComponent::hide() {
  m_isHidden = true;
}

bool rayc::DrawableComponent::getIsHidden() const {
  return m_isHidden;
}

int rayc::DrawableComponent::getWidth() const {
  return m_width;
}

int rayc::DrawableComponent::getHeight() const {
  return m_height;
}

int rayc::DrawableComponent::getX() const {
  return m_xOffset;
}

int rayc::DrawableComponent::getY() const {
  return m_yOffset;
}

void rayc::DrawableComponent::clearBuffer() {
  SDL_SetRenderTarget(getContext()->getRenderer(), m_buffer);
  SDL_SetRenderDrawColor(getContext()->getRenderer(), 0, 0, 0, 0);
  SDL_RenderClear(getContext()->getRenderer());
}
