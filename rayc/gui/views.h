#ifndef _RAYC_GUI_VIEW_H
#define _RAYC_GUI_VIEW_H

#include <string>
#include <vector>

#include "gui/object.h"
#include "res/colors.h"
#include "res/font.h"
#include "res/texture.h"

namespace rayc {
namespace gui {

class TextureView : public Object {
 public:
  TextureView(Texture* texture, Vec2i pos);
  TextureView(Texture* texture, Vec2i pos, int width, int height);
  ~TextureView();

  void draw(Vec2i abs_parent_offset) override;

 private:
  Texture* m_texture;
  SDL_Rect m_dest;
};

class TextView : public Object {
 private:
  struct Line {
    std::string text;
    SDL_Color color;
  };

 public:
  TextView(Font* font, int max_height);
  ~TextView();

  void pushLine(const std::string& line, SDL_Color color = kWhite);
  void popFront();
  void popBack();

  std::string& get(int i);
  int getSize() const;

  void draw(Vec2i abs_parent_offset) override;

 private:
  Font* m_font;
  int m_max_height;
  std::vector<Line> m_lines;
};

} /* namespace gui */
} /* namespace rayc */

#endif