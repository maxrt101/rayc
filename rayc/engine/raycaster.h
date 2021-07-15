#ifndef _RAYC_ENGINE_RAYCASTER_H
#define _RAYC_ENGINE_RAYCASTER_H

#include <list>
#include <vector>

#include "res/font.h"
#include "res/texture.h"
#include "math/vec2.h"
#include "engine/map_file.h"
#include "engine/object.h"

#define PI 3.14159f

namespace rayc {

class Raycaster {
 public:
  Raycaster(const std::string& datadir, const std::string& fontfile);
  ~Raycaster();

  bool loadMap(const std::string& filename);
  void unloadMap();

 public:
  /* res */
  std::string m_datadir;
  rayc::Font m_font_small;
  rayc::Font m_font_big;
  std::vector<Texture> m_textures;
  std::vector<Texture> m_sprites;
  std::list<std::pair<int, std::unique_ptr<Object>>> m_objects;
  rayc::Texture m_texture_no_texture;

  /* map */
  MapFile m_map;
  bool m_map_loaded = false;

  /* world */
  Vec2d m_player;
  float m_player_angle = 0.0f;
  float m_fov = PI / 2.5f;
  float m_depth = 30.0f;

  float m_step = 0.01f;
  int m_texture_column_width = 1;
  int m_floor_scale = 2;

  float m_rotation_speed = 4.0f;
  float m_movement_speed = 5.0f;

  SDL_Rect m_texture_src, m_texture_dest;
  float* m_depth_buffer = nullptr;

  // dda test
  rayc::Vec2d m_dir {-1, 0};
  rayc::Vec2d m_plane {0, 0.66};
};

} /* namespace rayc */

#endif