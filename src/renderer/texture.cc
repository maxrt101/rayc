#include <rayc/renderer/texture.h>
#include <rayc/exception.h>
#include <rayc/app.h>
#include <rayc/log.h>

#include <SDL2/SDL_image.h>

rayc::Texture::Texture() {}

rayc::Texture::Texture(const std::string& filename) : m_filename(filename) {
  m_texture = IMG_LoadTexture(app::getRenderer(), filename.c_str());
  if (!m_texture) {
    sdlError("Error loading texture '%s'", filename.c_str());
    throw FileOpenException(filename);
  } else {
    SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
  }
  debug("Texture(%s) %p", filename.c_str(), m_texture);
}

rayc::Texture::Texture(int w, int h, SDL_TextureAccess access) {
  m_width = w;
  m_height = h;
  m_texture = SDL_CreateTexture(app::getRenderer(), SDL_PIXELFORMAT_RGBA8888, access, w, h);
  if (!m_texture) {
    sdlError("Error creating texture %dx%d", w, h);
    throw ResourceCreationException("texture", "");
  }
  SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
}

rayc::Texture::Texture(const unsigned char* buffer, size_t bufferSize) {
  m_texture = IMG_LoadTexture_RW(app::getRenderer(), SDL_RWFromConstMem(buffer, bufferSize), true);
  if (!m_texture) {
    sdlError("Error loading texture %p(%zu)", buffer, bufferSize);
    throw ResourceCreationException("Texture", "");
  } else {
    SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
  }
  debug("Texture(%p %zu) %p", buffer, bufferSize, m_texture);
}

rayc::Texture::Texture(Texture&& rhs) {
  m_texture = rhs.m_texture;
  m_width = rhs.m_width;
  m_height = rhs.m_height;

  rhs.m_texture = nullptr;
}

rayc::Texture::~Texture() {
  if (m_texture) {
    SDL_DestroyTexture(m_texture);
  }
}

rayc::Texture& rayc::Texture::operator=(rayc::Texture&& rhs) {
  m_texture = rhs.m_texture;
  m_width = rhs.m_width;
  m_height = rhs.m_height;

  rhs.m_texture = nullptr;
  return *this;
}

rayc::Texture&& rayc::Texture::copy() const {
  return std::move(Texture(m_filename));
}

SDL_Texture* rayc::Texture::getSdlTexture() const {
  return m_texture;
}

int rayc::Texture::getWidth() const {
  return m_width;
}

int rayc::Texture::getHeight() const {
  return m_height;
}
