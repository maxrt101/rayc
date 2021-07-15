#include "engine/raycaster.h"
#include "core/app.h"
#include "utils/log.h"

rayc::Raycaster::Raycaster(const std::string& datadir, const std::string& fontfile)
    : m_datadir(datadir), m_font_small(datadir + fontfile), m_font_big(datadir + fontfile, 18), m_texture_no_texture(32, 32) {
  /* Fallback texture, which is used when requested texture is not available */
  SDL_Rect rect;
  rect.w = 16;
  rect.h = 16;
  SDL_SetRenderTarget(Application::getRenderer(), m_texture_no_texture.getSdlTexture());
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      rect.x = x * 16;
      rect.y = y * 16;
      if (x != y) {
        SDL_SetRenderDrawColor(Application::getRenderer(), 128, 0, 128, 255);
      } else {
        SDL_SetRenderDrawColor(Application::getRenderer(), 0, 0, 0, 255);
      }
      SDL_RenderFillRect(Application::getRenderer(), &rect);
    }
  }
  SDL_SetRenderTarget(Application::getRenderer(), NULL);
}

rayc::Raycaster::~Raycaster() {}

bool rayc::Raycaster::loadMap(const std::string& filename) {
  auto map = MapFile::load(m_datadir + filename);
  if (!map.exists()) {
    error("Error loading map file '%s'", (m_datadir + filename).c_str());
    return false;
  }

  unloadMap();
  m_map = map.get();

  m_player = { (double)m_map.getStartX(), (double)m_map.getStartY() };

  for (auto &texture_file : m_map.getTextures()) {
    m_textures.push_back(Texture(m_datadir + texture_file));
  }

  for (auto &sprite_file : m_map.getSprites()) {
    m_sprites.push_back(Texture(m_datadir + sprite_file));
  }

  for (MapObject& object : m_map.getObjects()) {
    float x = (float)object.x+0.5;
    float y = (float)object.y+0.5;
    Texture* texture = &m_texture_no_texture;
    if (object.sprite < m_sprites.size()) {
      texture = &m_sprites[object.sprite];
    }
    m_objects.push_back({0, std::unique_ptr<Object>(new Object({x,y}, {0,0}, texture)) });
  }

  info("Map '%s' loaded.", m_map.getName().c_str());
  m_map_loaded = true;
  return true;
}

void rayc::Raycaster::unloadMap() {
  m_map_loaded = false;
  m_textures.clear();
  m_sprites.clear();
  m_objects.clear();
}
