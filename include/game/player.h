#ifndef _RAYC_GAME_PLAYER_H_
#define _RAYC_GAME_PLAYER_H_ 1

#include <rayc/math/vec2.h>
#include <rayc/game/object.h>
#include <rayc/game/weapon.h>

namespace rayc {

struct Player : public Object {
  std::string name = "player";

  float angle = 0.0f;
  float rotationSpeed = 2.0f;
  float movementSpeed = 5.0f;

  std::vector<Weapon*> weapons;
  int currentWeapon = -1;

  // std::vector<std::string, int> inventory; // ?

 public:
  Weapon* getWeapon();
};

} /* namespace rayc */

#endif /* _RAYC_GAME_PLAYER_H_ */