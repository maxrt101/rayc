#include "layers/scenedda.h"
#include "utils/debug.h"

#include <cmath>
#include <iostream>

const std::string rayc::SceneDDALayer::Name = "scene";

rayc::SceneDDALayer::SceneDDALayer(Application* app, int w, int h, Raycaster* raycaster)
  : rayc::Layer(app, Name, w, h, 0, 0), m_raycaster(raycaster),
    m_update_layer(app, w, h, raycaster) {
  m_update_layer.show();
  m_sublayers.push_back(&m_update_layer);
  hide();

  m_raycaster->m_depth_buffer = new float[getWidth()];
}

rayc::SceneDDALayer::~SceneDDALayer() {}

bool rayc::SceneDDALayer::onDraw() {
  clearBuffer();
  SDL_Rect rect_src, rect_dest;

  Vec2d& pos = m_raycaster->m_player;
  Vec2d& plane = m_raycaster->m_plane;
  Vec2d& dir = m_raycaster->m_dir;

  for (int x = 0; x < getWidth(); x++) {
    double camera_x = 2 * x / (double)getWidth() - 1;
    Vec2d ray_dir {
      dir.x + plane.x * camera_x,
      dir.y + plane.y * camera_x,
    };

    Vec2i map {(int)pos.x, (int)pos.x};
    Vec2i step;
    Vec2d side_dist;
    Vec2d delta_dist {std::abs(1/ray_dir.x), std::abs(1/ray_dir.y)};

    double perp_wall_dist;

    int hit = 0;
    int side;

    if (ray_dir.x < 0) {
      step.x = -1;
      side_dist.x = (pos.x - map.x) * delta_dist.x;
    } else {
      step.x = 1;
      side_dist.x = (map.x + 1.0 - pos.x) * delta_dist.x;
    }

    if (ray_dir.y < 0) {
      step.y = -1;
      side_dist.y = (pos.y - map.y) * delta_dist.y;
    } else {
      step.y = 1;
      side_dist.y = (map.y + 1.0 - pos.y) * delta_dist.y;
    }

    while (!hit) {
      if (side_dist.x < side_dist.y) {
        side_dist.x += delta_dist.x;
        map.x += step.x;
        side = 0;
      } else {
        side_dist.y += delta_dist.y;
        map.y += step.y;
        side = 1;
      }

      if (m_raycaster->m_map.getTile(map.x, map.y).isSolid()) {
        hit = 1;
      }
    }

    double wall_x;
    if (side == 0) {
      perp_wall_dist = (map.x - pos.x + (1 - step.x) / 2) / ray_dir.x;
      wall_x = pos.y + perp_wall_dist * ray_dir.y;
    } else {
      perp_wall_dist = (map.y - pos.y + (1 - step.y) / 2) / ray_dir.y;
      wall_x = pos.x + perp_wall_dist * ray_dir.x;
    }
    wall_x -= floor(wall_x);

    int line_height = (int)(getHeight() / perp_wall_dist);

    int line_start = -line_height / 2 + getHeight() / 2;
    //if (line_start < 0) line_start = 0;
    int line_end = line_height / 2 + getHeight() / 2;
    //if (line_end >= getHeight()) line_end = getHeight() - 1;
    
    int texture_idx = m_raycaster->m_map.getTile(map.x, map.y).texture;
    // Texture* texture = &m_raycaster->m_texture_no_texture;
    // if (texture_idx < m_raycaster->m_textures.size()) {
    //   texture = &m_raycaster->m_textures[texture_idx];
    // }
    Texture* texture = &m_raycaster->m_textures[texture_idx];

    int sample_x = int(wall_x * double(texture->getWidth()));
    if (side == 0 && ray_dir.x > 0) {
      sample_x = texture->getWidth() - sample_x - 1;
    }
    if (side == 1 && ray_dir.y < 0) {
      sample_x = texture->getWidth() - sample_x - 1;
    }

    if (isDebugEnabled())
     printf("x: %d pos: {%.2lf %.2lf} map: {%d %d} sample: %d texture: %d (%p)\n", x, pos.x, pos.y, map.x, map.y, sample_x, texture_idx, texture);

    m_raycaster->m_depth_buffer[x] = perp_wall_dist;

    rect_src.x = sample_x;
    rect_src.y = 0;
    rect_src.w = m_raycaster->m_texture_column_width;
    rect_src.h = texture->getHeight();

    rect_dest.x = x;
    rect_dest.y = line_start;
    rect_dest.w = m_raycaster->m_texture_column_width;
    rect_dest.h = line_end - line_start;

    SDL_RenderCopy(getRenderer(), texture->getSdlTexture(), &rect_src, &rect_dest);
  }

  return true;
}

void rayc::SceneDDALayer::onFrameUpdate(float frame_time) {}

void rayc::SceneDDALayer::onShow() {}

void rayc::SceneDDALayer::onHide() {}
