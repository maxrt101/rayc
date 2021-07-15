#include "layers/scene.h"
#include "core/app.h"
#include "layers/console.h"

#include <cmath>

const std::string rayc::SceneLayer::Name = "scene";

rayc::SceneLayer::SceneLayer(Application* app, int w, int h, Raycaster* raycaster)
  : rayc::Layer(app, Name, w, h, 0, 0), m_raycaster(raycaster),
    m_update_layer(app, w, h, raycaster) {
  m_update_layer.show();
  m_sublayers.push_back(&m_update_layer);
  hide();

  m_raycaster->m_depth_buffer = new float[getWidth()];
}

rayc::SceneLayer::~SceneLayer() {}

bool rayc::SceneLayer::onDraw() {
  clearBuffer();

  int screen_height = getHeight();
  int screen_width = getWidth();
  int map_height = m_raycaster->m_map.getHeight();
  int map_width = m_raycaster->m_map.getWidth();

  SDL_Rect rect_src, rect_dest;

  rect_dest.x = 0;
  rect_dest.y = screen_height/2;
  rect_dest.w = screen_width;
  rect_dest.h = screen_height/2;

  SDL_SetRenderDrawColor(getRenderer(), 128, 128, 128, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(getRenderer(), &rect_dest);

  // Wall Rendering
  for (int x = 0; x < screen_width; x+=m_raycaster->m_texture_column_width) {
    float ray_angle = (m_raycaster->m_player_angle - m_raycaster->m_fov/2.0f) + ((float)x / (float)screen_width) * m_raycaster->m_fov;
    float distance_to_wall = 0;
    float perp_distance_to_wall;

    bool hit_wall = false;

    rayc::Vec2f eye(
      sinf(ray_angle),
      cosf(ray_angle)
    );

    rayc::Vec2i test(0, 0);
    int side = 0;

    float sample_x = 0;

    while (!hit_wall && distance_to_wall < m_raycaster->m_depth) {
      distance_to_wall += m_raycaster->m_step;

      test.x = m_raycaster->m_player.x + eye.x * distance_to_wall;
      test.y = m_raycaster->m_player.y + eye.y * distance_to_wall;

      if (test.x < 0 || test.x > map_width || test.y < 0 || test.y > map_height) {
        hit_wall = true;
        distance_to_wall = m_raycaster->m_depth;
      } else {
        if (m_raycaster->m_map.getTile(test).isSolid()) {
          hit_wall = true;

          rayc::Vec2f block_mid(
            test.x + 0.5f,
            test.y + 0.5f
          );

          rayc::Vec2f test_point(
            m_raycaster->m_player.x + eye.x * distance_to_wall,
            m_raycaster->m_player.y + eye.y * distance_to_wall
          );

          float test_angle = atan2f((test_point.y - block_mid.y), (test_point.x - block_mid.x));

          if (test_angle >= -PI * 0.25f && test_angle < PI * 0.25f) {
            sample_x = test_point.y - test.y;
            side = 1;
          }
          if (test_angle >= PI * 0.25f && test_angle < PI * 0.75f) {
            sample_x = test_point.x - test.x;
            side = 0;
          }
          if (test_angle < -PI * 0.25f && test_angle >= -PI * 0.75f) {
            sample_x = test_point.x - test.x;
            side = 0;
          }
          if (test_angle >= PI * 0.75f || test_angle < -PI * 0.75f) {
            sample_x = test_point.y - test.y;
            side = 1;
          }

          perp_distance_to_wall = distance_to_wall * cosf(ray_angle - m_raycaster->m_player_angle);
        }
      }
    }

    int y_start = (float)(screen_height / 2.0f) - screen_height / ((float)perp_distance_to_wall) / 2.0;
    int y_end = y_start + screen_height/perp_distance_to_wall;

    int texture_idx = m_raycaster->m_map.getTile(test).texture;
    Texture* texture = &m_raycaster->m_texture_no_texture;
    if (texture_idx < m_raycaster->m_textures.size()) {
      texture = &m_raycaster->m_textures[texture_idx];
    }

    float whole;
    int texture_x = (std::modf(sample_x, &whole) * texture->getWidth());

    m_raycaster->m_depth_buffer[x] = perp_distance_to_wall;

    rect_src.x = texture_x;
    rect_src.y = 0;
    rect_src.w = m_raycaster->m_texture_column_width;
    rect_src.h = texture->getHeight();

    rect_dest.x = x;
    rect_dest.y = y_start;
    rect_dest.w = m_raycaster->m_texture_column_width;
    rect_dest.h = (float)screen_height/perp_distance_to_wall;

    SDL_RenderCopy(getRenderer(), texture->getSdlTexture(), &rect_src, &rect_dest);
  }

  // Sprites Rendering
  for (auto &pair : m_raycaster->m_objects) {
    auto& obj = *pair.second.get();
    /** TODO: Rewrite wihout RTTI */
    /*if (typeid(obj).hash_code() == typeid(Projectile).hash_code()
      || typeid(obj).hash_code() == typeid(BouncyProjectile).hash_code()
      || typeid(obj).hash_code() == typeid(MovingObject).hash_code()) {
      pair.second->pos.x += dynamic_cast<Projectile*>(pair.second.get())->v.x * frame_time;
      pair.second->pos.y += dynamic_cast<Projectile*>(pair.second.get())->v.y * frame_time;
    }*/

    // pair.second->OnFrameUpdate();

    /*if (GetMapTile(pair.second->pos.x, pair.second->pos.y).IsSolid()) {
      pair.second->OnWallCollision();
    }*/

    Vec2f vec(
      pair.second->getPos().x - m_raycaster->m_player.x,
      pair.second->getPos().y - m_raycaster->m_player.y
    );

    float distance_from_player = sqrtf(vec.x*vec.x + vec.y*vec.y);

    Vec2f eye(
      sinf(m_raycaster->m_player_angle),
      cosf(m_raycaster->m_player_angle)
    );

    float object_angle = atan2f(eye.y, eye.x) - atan2f(vec.y, vec.x);
    if (object_angle < -PI) {
      object_angle += 2.0f * PI;
    }
    if (object_angle > PI) {
      object_angle -= 2.0f * PI;
    }
    
    distance_from_player = distance_from_player * cosf(object_angle);
    pair.first = distance_from_player;

    bool is_in_fov = fabs(object_angle) < m_raycaster->m_fov / 2.0f;

    if (is_in_fov && distance_from_player >= 0.5f && distance_from_player < m_raycaster->m_depth) {
      float object_ceiling = ((float)screen_height / 2.0f) - screen_height/distance_from_player/1.5f;
      float object_floor = screen_height - object_ceiling;
      float object_height = object_floor - object_ceiling;
      float object_aspect_ratio = (float)pair.second->getTexture()->getHeight() / (float)pair.second->getTexture()->getWidth();
      float object_width = object_height/object_aspect_ratio;
      float object_middle = (0.5f * (object_angle / (m_raycaster->m_fov / 2.0f)) + 0.5f) * (float)screen_width;
    
      float whole;

      for (int sx = 0; sx < object_width; sx++) {
        int object_column = object_middle + sx - (object_width/2.0f);

        rect_src.x = modf(sx / object_width, &whole) * pair.second->getTexture()->getWidth();
        rect_src.y = 0;
        rect_src.w = 1;
        rect_src.h = pair.second->getTexture()->getHeight();

        rect_dest.x = object_column;
        rect_dest.y = object_ceiling;
        rect_dest.w = 1;
        rect_dest.h = object_height;

        if (m_raycaster->m_depth_buffer[object_column] >= distance_from_player) {
          SDL_RenderCopy(getRenderer(), pair.second->getTexture()->getSdlTexture(), &rect_src, &rect_dest);
        }
      }
    }
  }

  return true;
}

void rayc::SceneLayer::onFrameUpdate(float frame_time) {
  if (getApp()->getKeyState(SDL_SCANCODE_GRAVE).pressed) {
    getApp()->getLayer(rayc::ConsoleLayer::Name)->show();
  }
}

void rayc::SceneLayer::onShow() {}

void rayc::SceneLayer::onHide() {}
