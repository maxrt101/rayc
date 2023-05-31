#ifndef _RAYC_GAME_OBJECT_H_
#define _RAYC_GAME_OBJECT_H_ 1

#include <rayc/math/vec2.h>
#include <rayc/game/entity.h>

#include <string>

namespace rayc {

struct Object {
  Entity* entity = nullptr;

  Vec2d position = {0, 0};
  Vec2d velocity = {0, 0};

  bool visible = true;
  bool remove = false;

  int health;

  EntityState state;
  int currentAnimationFrame = 0;
  int timeFromAnimationStart = 0;

 public:
  static Object* fromEntity(Entity* entity); 

  std::string getCurrentTexture();

  /*
  virtual void onCollision(Object* collided);
  virtual void onFrameUpdate(float frameTile);
  */
};

} /* namespace rayc */

#endif /* _RAYC_GAME_OBJECT_H_ */