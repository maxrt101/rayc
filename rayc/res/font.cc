#include "res/font.h"
#include "core/app.h"
#include "utils/log.h"

const int rayc::Font::DefaultFontSize = 14;

rayc::Font::Font() {}

rayc::Font::Font(const std::string& filename) : Font(filename, DefaultFontSize) {}

rayc::Font::Font(const std::string& filename, int size) {
  m_filename = filename;
  m_size = size;
  m_font = TTF_OpenFont(filename.c_str(), size);
  if (!m_font) {
    error("Cannot open '%s' font file", filename.c_str());
  }
  debug("rayc::Font(%s, %d) %p", filename.c_str(), size, m_font); // debug
}

rayc::Font::Font(Font&& rhs) {
  this->m_filename = rhs.m_filename;
  this->m_size = rhs.m_size;
  this->m_font = rhs.m_font;

  rhs.m_font = nullptr;
}

rayc::Font::~Font() {
  if (m_font) {
    TTF_CloseFont(m_font);
  }
}

rayc::Font&& rayc::Font::copy() const {
  return std::move(Font(m_filename, m_size));
}

TTF_Font* rayc::Font::getFont() const {
  return m_font;
}

int rayc::Font::getSize() const {
  return m_size;
}

void rayc::Font::draw(const char* text, Vec2i pos, SDL_Color color) {
  if (!m_font || !text) {
    return;
  }

  SDL_Surface* surface = TTF_RenderText_Solid(m_font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::getRenderer(), surface);
  SDL_Rect dest;

  dest.x = pos.x;
  dest.y = pos.y;
  dest.w = surface->w;
  dest.h = surface->h;

  SDL_RenderCopy(Application::getRenderer(), texture, NULL, &dest);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}