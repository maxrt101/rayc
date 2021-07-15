#include "engine/object.h"

rayc::Object::Object(Vec2d pos, Texture* texture) {
  m_pos = pos;
  m_texture = texture;
}

rayc::Object::Object(Vec2d pos, Vec2d vel, Texture* texture) {
  m_pos = pos;
  m_velocity = vel;
  m_texture = texture;
}

rayc::Vec2d rayc::Object::getPos() const {
  return m_pos;
}

void rayc::Object::setPos(Vec2d pos) {
  m_pos = pos;
}

rayc::Vec2d rayc::Object::getVelocity() const {
  return m_velocity;
}

void rayc::Object::setVelocity(Vec2d vel) {
  m_velocity = vel;
}

rayc::Texture* rayc::Object::getTexture() const {
  return m_texture;
}

void rayc::Object::setTexture(Texture* texture) {
  m_texture = texture;
}

void rayc::Object::hide() {
  m_visible = false;
}

void rayc::Object::show() {
  m_visible = true;
}

void rayc::Object::remove() {
  m_remove = true;
}

bool rayc::Object::isRemovable() const {
  return m_remove;
}

void rayc::Object::onWallCollision() {
  remove();
}

void rayc::Object::onFrameUpdate() {}