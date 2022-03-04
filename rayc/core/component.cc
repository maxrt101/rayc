#include "core/component.h"

#include <cstdlib>
#include "utils/shared_library.h"
#include "utils/log.h"
#include "utils/die.h"

rayc::Component::Component(Context* ctx, int priority, const std::string& name)
  : Component(ctx, priority, name, 0, 0, 0, 0) {}

rayc::Component::Component(Context* ctx, int priority, const std::string& name, int width, int height, int x = 0, int y = 0)
    : m_ctx(ctx), m_priority(priority), m_name(name), m_width(width), m_height(height), m_xOffset(x), m_yOffset(y) {
  if (!ctx) {
    fatal("Context is null (while creating Component@%p:'%s')", this, m_name.c_str());
    die();
  }
  m_isAttached = true;
  onAttach();
}

rayc::Component::~Component() {}

bool rayc::Component::isAttached() const {
  return m_isAttached;
}

void rayc::Component::reattach(Context* ctx) {
  if (!ctx) {
    fatal("Context is null (while attaching Component@%p:'%s')", this, m_name.c_str());
    die();
  }
  m_isAttached = true;
  onAttach();
}

void rayc::Component::detach() {
  m_isAttached = false;
  onDetach();
}

bool rayc::Component::isActive() const {
  return m_isActive;
}

void rayc::Component::setActive(bool active) {
  m_isActive = active;
}

int rayc::Component::getPriority() const {
  return m_priority;
}

void rayc::Component::setPriority(int priority) {
  m_priority = priority;
}

std::string rayc::Component::getName() const {
  return m_name;
}

rayc::Context* rayc::Component::getContext() const {
  return m_ctx;
}

bool rayc::Component::isHidden() const {
  return m_isHidden;
}

void rayc::Component::show() {
  m_isHidden = false;
}

void rayc::Component::hide() {
  m_isHidden = true;
}

void rayc::Component::draw() const {
  SDL_SetRenderTarget(getContext()->getRenderer(), m_buffer);

  SDL_Rect dest;
  dest.x = m_xOffset;
  dest.y = m_yOffset;
  dest.w = m_width;
  dest.h = m_height;

  SDL_SetRenderTarget(getContext()->getRenderer(), NULL);
  SDL_RenderCopy(getContext()->getRenderer(), m_buffer, NULL, &dest);
}

int rayc::Component::getWidth() const {
  return m_width;
}

int rayc::Component::getHeight() const {
  return m_height;
}

void rayc::Component::move(int x, int y) {
  m_xOffset = x;
  m_yOffset = y;
}

int rayc::Component::getX() const {
  return m_xOffset;
}

int rayc::Component::getY() const {
  return m_yOffset;
}

void rayc::Component::update(float frameTime) {
  if (isAttached() && getPriority() > 0) {
    if (!isHidden()) {
      onDraw();
    }
    if (isActive()) {
      onFrameUpdate(frameTime);
    }
  }
}

void rayc::Component::clearBuffer() const {
  SDL_SetRenderTarget(getContext()->getRenderer(), m_buffer);
  SDL_SetRenderDrawColor(getContext()->getRenderer(), 0, 0, 0, 0);
  SDL_RenderClear(getContext()->getRenderer());
}

rayc::Component* rayc::Component::fromFile(const std::string& filename, Context* ctx) {
  SharedLibrary lib = SharedLibrary::load(filename);
  auto createFunction = lib.getSymbolAs<ComponentFunction>(RAYC_COMPONENT_CREATE_FUNCTION_STR);
  if (!createFunction) {
    fatal("Component File '%s' doesn't contain a compoent create function (symbol '%s')", filename.c_str(), RAYC_COMPONENT_CREATE_FUNCTION_STR);
    die();
  }
  return createFunction(ctx);
}

void rayc::Component::onAttach() {}

void rayc::Component::onDetach() {}

void rayc::Component::onDraw() {}

void rayc::Component::onFrameUpdate(float frameTime) {}
