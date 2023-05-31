#include <rayc/game/object.h>

rayc::Object* rayc::Object::fromEntity(Entity* entity) {
  Object* object = new Object;

  object->entity = entity;
  object->health = entity->spawnHealth;
  object->state = EntityState::IDLE;

  return object;
}

std::string rayc::Object::getCurrentTexture() {
  return entity->animations[state].frames[currentAnimationFrame];
}
