#ifndef _RAYC_GUI_OBJECT_H
#define _RAYC_GUI_OBJECT_H

#include "math/vec2.h"

namespace rayc {
namespace gui {

class Object {
 public:
  Object();
  Object(Vec2i pos, int width, int height);
  virtual ~Object();

  void init(Vec2i pos, int width, int height);

  int getWidth() const;
  int getHeight() const;
  int getX() const;
  int getY() const;

  virtual void draw(Vec2i abs_parent_offset);

 protected:
  Vec2i m_pos;
  int m_width;
  int m_height;
};

} /* namespace gui */
} /* namespace rayc */

#endif