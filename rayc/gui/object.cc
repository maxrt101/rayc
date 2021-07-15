#include "gui/object.h"

rayc::gui::Object::Object() {}

rayc::gui::Object::Object(Vec2i pos, int width, int height) {
  init(pos, width, height);
}

rayc::gui::Object::~Object() {}

void rayc::gui::Object::init(rayc::Vec2i pos, int width, int height) {
  m_pos = pos;
  m_width = width;
  m_height = height;
}

int rayc::gui::Object::getWidth() const {
  return m_width;
}

int rayc::gui::Object::getHeight() const {
  return m_height;
}

int rayc::gui::Object::getX() const {
  return m_pos.x;
}

int rayc::gui::Object::getY() const {
  return m_pos.y;
}

void rayc::gui::Object::draw(Vec2i abs_parent_offset) {}