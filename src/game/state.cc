#include <rayc/game/state.h>

static rayc::Player gPlayer;

rayc::Player& rayc::getPlayer() {
  return gPlayer;
}
