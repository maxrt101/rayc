#ifndef _RAYC_ENGINE_OBJECT_H
#define _RAYC_ENGINE_OBJECT_H

#include "res/texture.h"
#include "math/vec2.h"

namespace rayc {

class Object {
 public:
  Object(Vec2d pos, Texture* texture);
  Object(Vec2d pos, Vec2d vel, Texture* texture);

  Vec2d getPos() const;
  void setPos(Vec2d pos);
  Vec2d getVelocity() const;
  void setVelocity(Vec2d vel);
  Texture* getTexture() const;
  void setTexture(Texture* texture);

  void hide();
  void show();
  void remove();
  bool isRemovable() const;

  virtual void onWallCollision();
  virtual void onFrameUpdate();

 private:
   // uint32_t id;
  Vec2d m_pos;
  Vec2d m_velocity;

  bool m_visible;
  bool m_remove;

  Texture* m_texture;
};

} /* namespace rayc */

#endif