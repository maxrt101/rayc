#ifndef _RAYC_RENDERER_FONT_H_
#define _RAYC_RENDERER_FONT_H_ 1

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <rayc/math/vec2.h>

namespace rayc {

class Font {
 private:
  TTF_Font* m_font = nullptr;
  int m_size = 14;
  int m_width = 0, m_height = 0; 

 public:
  Font() = default;
  Font(const std::string& filename);
  Font(const std::string& filename, int size);
  Font(const unsigned char* buffer, size_t bufferSize, int size = 14);

  TTF_Font* getTTF() const;
  int getSize() const;
  int getWidth() const;
  int getHeight() const;

  void draw(const std::string& text, Vec2i pos, SDL_Color color);
};

} /* namespace rayc */

#endif /* _RAYC_RENDERER_FONT_H_ */