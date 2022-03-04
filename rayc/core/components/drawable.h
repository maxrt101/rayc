#ifndef _RAYC_CORE_COMPONENTS_DRAWABLE_H
#define _RAYC_CORE_COMPONENTS_DRAWABLE_H

#include "core/component.h"
#include "core/context.h"

namespace rayc {

class DrawableComponent : public Component {
 private:
  SDL_Texture* m_buffer = nullptr;
  std::string m_name;
  bool m_isHidden = false;
  int m_width = 0;
  int m_height = 0;
  int m_xOffset = 0;
  int m_yOffset = 0;

 public:
  DrawableComponent(Context* ctx, int priority, const std::string& name, int width, int height, int x = 0, int y = 0);
  ~DrawableComponent();

  void draw() const;

  void move(int x, int y);
  void show();
  void hide();

  bool getIsHidden() const;
  int getWidth() const;
  int getHeight() const;
  int getX() const;
  int getY() const;

 public:
  virtual void onDraw() = 0;

 protected:
  void clearBuffer();
};

}; /* namespace rayc */

#endif
