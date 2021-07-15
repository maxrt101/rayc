#ifndef _RAYC_GUI_LAYOUT_H
#define _RAYC_GUI_LAYOUT_H

#include <vector>

#include "math/vec2.h"
#include "gui/object.h"

#define RAYC_GUI_POS_AUTO Vec2i(-1, -1)

namespace rayc {
namespace gui {

class Layout {
 public:
  Layout();
  Layout(Vec2i pos);
  virtual ~Layout();

  Vec2i getPos() const;
  void setPos(Vec2i pos);

  void addObject(Object* object);
  void removeObject(int i);
  void clear();

  virtual void draw(Vec2i abs_parent_offset);

 protected:
  Vec2i m_pos;
  std::vector<Object*> m_objects;
};

class VecticalLayout : public Layout {
 public:
  void draw(Vec2i abs_parent_offset) override;
};

class HorizontalLayout : public Layout {
 public:
  void draw(Vec2i abs_parent_offset) override;
};

} /* namespace gui */
} /* namespace rayc */

#endif