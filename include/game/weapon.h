#ifndef _RAYC_GAME_WEAPON_H_
#define _RAYC_GAME_WEAPON_H_ 1

#include <string>
#include <map>

#include <rayc/game/animation.h>
#include <rayc/game/entity.h>
#include <rayc/config.h>

namespace rayc {

enum class WeaponState {
  IDLE,
  FIRING,
  RELOADING,
};

struct Weapon {
  std::string tag;
  std::string displayName;

  AttackType attackType;
  int damage = 1; // damage per shot
  int fireRate = 1; // shots per tick/s?
  std::string projectileType;

  // TODO: ammo?

  std::map<WeaponState, Animation> animations;

 public:
  static Weapon* fromConfig(const Config& config);
};

} /* namespace rayc */

#endif /* _RAYC_GAME_WEAPON_H_ */