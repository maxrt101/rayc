#include <rayc/renderer/font.h>
#include <rayc/math/rect.h>
#include <rayc/exception.h>
#include <rayc/app.h>
#include <rayc/log.h>

rayc::Font::Font(const std::string& filename) : Font(filename, 14) {}

rayc::Font::Font(const std::string& filename, int size) {
  m_size = size;
  m_font = TTF_OpenFont(filename.c_str(), size);
  if (!m_font) {
    sdlError("Failed to open font");
    throw FileOpenException(filename);
  }
  TTF_SizeText(m_font, "A", &m_width, &m_height);
  debug("Font(%s) %p", filename.c_str(), m_font);
}

rayc::Font::Font(const unsigned char* buffer, size_t bufferSize, int size) {
  m_size = size;
  m_font = TTF_OpenFontRW(SDL_RWFromConstMem(buffer, bufferSize), 0, m_size);
  if (!m_font) {
    sdlError("Failed to open font");
    throw ResourceCreationException("Font", "");
  }
  TTF_SizeText(m_font, "A", &m_width, &m_height);
  debug("Font(%p, %zu) %p", buffer, bufferSize, m_font);
}

TTF_Font* rayc::Font::getTTF() const {
  return m_font;
}

int rayc::Font::getSize() const {
  return m_size;
}

int rayc::Font::getWidth() const {
  return m_width;
}

int rayc::Font::getHeight() const {
  return m_height;
}

void rayc::Font::draw(const std::string& text, Vec2i pos, SDL_Color color) {
  if (!m_font || text.empty()) {
    return;
  }

  SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(app::getRenderer(), surface);

  Rect dest = {pos.x, pos.y, surface->w, surface->h};
  SDL_Rect destSdl = dest.toSdlRect();

  SDL_RenderCopy(app::getRenderer(), texture, NULL, &destSdl);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}
