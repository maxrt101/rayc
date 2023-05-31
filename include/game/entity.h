#ifndef _RAYC_GAME_ENTITY_H_
#define _RAYC_GAME_ENTITY_H_ 1

#include <string>
#include <vector>
#include <map>

#include <rayc/game/animation.h>
#include <rayc/config.h>

namespace rayc {

enum class EntityState {
  IDLE,
  MOVING,
  ATTACKING,
  TAKING_DAMAGE,
  DEAD
};

enum class AttackType {
  NONE,
  MELLEE,
  RANGED,
  HITSCAN
};

enum class OnCollistionAction {
  STOP,
  REMOVE,
  BOUNCE,
};

struct Entity {
  std::string tag;
  std::string displayName;

  // TODO: entity flags

  int spawnHealth = 1; // -1 - can't die

  int attackDamage = 0;
  AttackType attackType = AttackType::NONE;
  std::string projectileType;

  OnCollistionAction onCollistion;

  std::map<EntityState, Animation> animations;

 public:
  static Entity* fromConfig(const Config& config);
};

} /* namespace rayc */

#endif /* _RAYC_GAME_ENTITY_H_ */