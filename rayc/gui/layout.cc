#include "gui/layout.h"

rayc::gui::Layout::Layout() {}

rayc::gui::Layout::Layout(Vec2i pos) {
  m_pos = pos;
}

rayc::gui::Layout::~Layout() {}

rayc::Vec2i rayc::gui::Layout::getPos() const {
  return m_pos;
}

void rayc::gui::Layout::setPos(Vec2i pos) {
  m_pos = pos;
}

void rayc::gui::Layout::addObject(Object* object) {
  m_objects.push_back(object);
}

void rayc::gui::Layout::removeObject(int i) {
  m_objects.erase(m_objects.begin()+i);
}

void rayc::gui::Layout::clear() {
  m_objects.clear();
}

void rayc::gui::Layout::draw(Vec2i abs_parent_offset) {
  Vec2i offset = m_pos;
  offset.x += abs_parent_offset.x;
  offset.y += abs_parent_offset.y;
  for (auto& object : m_objects) {
    object->draw(offset);
  }
}

void rayc::gui::VecticalLayout::draw(Vec2i abs_parent_offset) {
  Vec2i offset = m_pos;
  offset.x += abs_parent_offset.x;
  offset.y += abs_parent_offset.y;
  Vec2i last_pos;
  int last_height;
  for (auto& object : m_objects) {
    if (object->getX() == -1 && object->getY() == -1) {
      last_pos.y += last_height;
      object->draw({offset.x + last_pos.x + 1, offset.y + last_pos.y + 1});
    } else {
      object->draw(offset);
      last_pos = {object->getX(), object->getY()};
    }
    last_height = object->getHeight();
  }
}

void rayc::gui::HorizontalLayout::draw(Vec2i abs_parent_offset) {
  Vec2i offset = m_pos;
  offset.x += abs_parent_offset.x;
  offset.y += abs_parent_offset.y;
  Vec2i last_pos;
  int last_width;
  for (auto& object : m_objects) {
    if (object->getX() == -1 && object->getY() == -1) {
      last_pos.x += last_width;
      object->draw({offset.x + last_pos.x + 1, offset.y + last_pos.y + 1});
    } else {
      object->draw(offset);
      last_pos = {object->getX(), object->getY()};
    }
    last_width = object->getWidth();
  }
}
