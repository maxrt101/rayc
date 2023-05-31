#include <rayc/game/entity.h>
#include <rayc/exception.h>
#include <rayc/utils/string.h>

/*
tag=
author=
name=
animation.
animation.idle.
animation.idle.frames=
animation.idle.time=
*/

rayc::Entity* rayc::Entity::fromConfig(const Config& config) {
  if (!config.exists("tag")) {
    throw MissingConfigEntryException("tag");
  }

  if (!config.exists("author")) {
    throw MissingConfigEntryException("author");
  }

  if (!config.exists("name")) {
    throw MissingConfigEntryException("name");
  }

  Entity* entity = new Entity;

  entity->tag = config.getValue("tag");
  entity->displayName = config.getValue("name");

  if (config.exists("animation.idle.frames")) {
    entity->animations[EntityState::IDLE] = {1, strsplit(config.getValue("animation.idle.frames"), ",")};
  }

  // TODO: finish

  return entity;
}
