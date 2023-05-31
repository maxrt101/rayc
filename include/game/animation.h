#ifndef _RAYC_GAME_ANIMATION_H_
#define _RAYC_GAME_ANIMATION_H_ 1

#include <string>
#include <vector>

namespace rayc {

struct Animation {
  int transitionTime;
  std::vector<std::string> frames;
};

} /* namespace rayc */

#endif /* _RAYC_GAME_ANIMATION_H_ */