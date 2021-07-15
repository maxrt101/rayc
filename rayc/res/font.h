#ifndef _RAYC_RES_FONT_H 
#define _RAYC_RES_FONT_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "math/vec2.h"
#include "res/colors.h"

namespace rayc {

class Font {
 public:
  Font();
  Font(const std::string& filename);
  Font(const std::string& filename, int size);
  Font(const Font& rhs) = delete;
  Font(Font&& rhs);
  ~Font();

  Font&& copy() const;

  TTF_Font* getFont() const;
  int getSize() const;

  void draw(const char* text, Vec2i pos, SDL_Color color = rayc::kWhite);

  static const int DefaultFontSize;

 private:
  std::string m_filename;
  TTF_Font* m_font;
  int m_size;
};

} /* namespace rayc */

#endif