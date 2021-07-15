#ifndef _RAYC_RES_TEXTURE_H
#define _RAYC_RES_TEXTURE_H

#include <string>

#include <SDL2/SDL.h>

namespace rayc {

class Texture {
 public:
  Texture();
  Texture(const std::string& filename);
  Texture(int w, int h);
  Texture(const Texture& rhs) = delete;
  Texture(Texture&& rhs);
  ~Texture();

  Texture& operator=(Texture&& rhs);

  Texture&& copy() const;

  SDL_Texture* getSdlTexture() const;
  // SDL_Surface* getPixels() const;

  int getWidth() const;
  int getHeight() const;

 private:
  std::string m_filename;
  SDL_Texture* m_texture = nullptr;
  int m_width = 0;
  int m_height = 0;
};

} /* namespace rayc */

#endif