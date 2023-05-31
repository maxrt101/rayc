#include <rayc/game/player.h>

rayc::Weapon* rayc::Player::getWeapon() {
  if (currentWeapon >= 0 && currentWeapon < weapons.size()) {
    return weapons[currentWeapon];
  }
  return nullptr;
}
