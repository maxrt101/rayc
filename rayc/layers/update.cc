#include "layers/update.h"
#include "core/app.h"
#include "layers/console.h"
#include "utils/debug.h"

const std::string rayc::UpdateLayer::Name = "scene:update";

rayc::UpdateLayer::UpdateLayer(Application* app, int w, int h, Raycaster* raycaster)
  : rayc::Layer(app, Name, w, h, 0, 0), m_raycaster(raycaster) {}

rayc::UpdateLayer::~UpdateLayer() {}

bool rayc::UpdateLayer::onDraw() {
  if (isDebugEnabled()) {
    clearBuffer();
    char fps_buf[5] {0};
    snprintf(fps_buf, 5, "%4d", int(1/m_last_frame_time));
    m_raycaster->m_font_small.draw(fps_buf, {getWidth() -  m_raycaster->m_font_small.getSize() * 4 , 0});
    return true;
  }
  return false;
}

void rayc::UpdateLayer::onFrameUpdate(float frame_time) {
  m_last_frame_time = frame_time;

  if (getApp()->getKeyState(SDL_SCANCODE_GRAVE).pressed) {
    getApp()->getLayer(rayc::ConsoleLayer::Name)->show();
  }

  // Movement
  /*if (getApp()->getKeyState(SDL_SCANCODE_LEFT).held) {
    m_raycaster->m_player_angle -= m_raycaster->m_rotation_speed * frame_time;
  }

  if (getApp()->getKeyState(SDL_SCANCODE_RIGHT).held) {
    m_raycaster->m_player_angle += m_raycaster->m_rotation_speed * frame_time;
  }

  if (getApp()->getKeyState(SDL_SCANCODE_W).held) {
    m_raycaster->m_player.x += sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    m_raycaster->m_player.y += cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;

    if (m_raycaster->m_map.getTile((int)m_raycaster->m_player.x, (int)m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x -= sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
      m_raycaster->m_player.y -= cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    }
  }
  
  if (getApp()->getKeyState(SDL_SCANCODE_S).held) {
    m_raycaster->m_player.x -= sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    m_raycaster->m_player.y -= cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;

    if (m_raycaster->m_map.getTile((int)m_raycaster->m_player.x, (int)m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x += sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
      m_raycaster->m_player.y += cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    }
  }

  if (getApp()->getKeyState(SDL_SCANCODE_A).held) {
    m_raycaster->m_player.x -= cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    m_raycaster->m_player.y += sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;

    if (m_raycaster->m_map.getTile((int)m_raycaster->m_player.x, (int)m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x += cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
      m_raycaster->m_player.y -= sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    }
  }
  
  if (getApp()->getKeyState(SDL_SCANCODE_D).held) {
    m_raycaster->m_player.x += cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    m_raycaster->m_player.y -= sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;

    if (m_raycaster->m_map.getTile((int)m_raycaster->m_player.x, (int)m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x -= cosf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
      m_raycaster->m_player.y += sinf(m_raycaster->m_player_angle) * m_raycaster->m_movement_speed * frame_time;
    }
  }*/

  /*if (getApp()->getKeyState(SDL_SCANCODE_SPACE).pressed) {
    objects_.push_back({0,
      std::unique_ptr<Projectile>(
        new Projectile(
          m_player,
          mrt::Vec2f(sinf(m_player_angle) * 5, cosf(m_player_angle) * 5),
          &sprites_[0])
    )});
  }*/

  double move_speed = m_raycaster->m_movement_speed * frame_time;
  double rotation_speed = m_raycaster->m_rotation_speed * frame_time;

  if (getApp()->getKeyState(SDL_SCANCODE_W).held) {
    Vec2i pos(
      m_raycaster->m_player.x + m_raycaster->m_dir.x * move_speed,
      m_raycaster->m_player.y + m_raycaster->m_dir.y * move_speed
    );
    if (!m_raycaster->m_map.getTile(pos.x, m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x += m_raycaster->m_dir.x * move_speed;
    }
    if (!m_raycaster->m_map.getTile(m_raycaster->m_player.x, pos.y).isSolid()) {
      m_raycaster->m_player.y += m_raycaster->m_dir.y * move_speed;
    }
  }

  if (getApp()->getKeyState(SDL_SCANCODE_S).held) {
    Vec2i pos(
      m_raycaster->m_player.x - m_raycaster->m_dir.x * move_speed,
      m_raycaster->m_player.y - m_raycaster->m_dir.y * move_speed
    );
    if (!m_raycaster->m_map.getTile(pos.x, m_raycaster->m_player.y).isSolid()) {
      m_raycaster->m_player.x -= m_raycaster->m_dir.x * move_speed;
    }
    if (!m_raycaster->m_map.getTile(m_raycaster->m_player.x, pos.y).isSolid()) {
      m_raycaster->m_player.y -= m_raycaster->m_dir.y * move_speed;
    }
  }

  if (getApp()->getKeyState(SDL_SCANCODE_LEFT).held) {
    double old_dir_x = m_raycaster->m_dir.x;
    m_raycaster->m_dir.x = m_raycaster->m_dir.x * cos(rotation_speed) - m_raycaster->m_dir.y * sin(rotation_speed);
    m_raycaster->m_dir.y = old_dir_x * sin(rotation_speed) + m_raycaster->m_dir.y * cos(rotation_speed);
    double old_plane_x = m_raycaster->m_plane.x;
    m_raycaster->m_plane.x = m_raycaster->m_plane.x * cos(rotation_speed) - m_raycaster->m_plane.y * sin(rotation_speed);
    m_raycaster->m_plane.y = old_plane_x * sin(rotation_speed) + m_raycaster->m_plane.y * cos(rotation_speed);
  }

  if (getApp()->getKeyState(SDL_SCANCODE_RIGHT).held) {
    double old_dir_x = m_raycaster->m_dir.x;
    m_raycaster->m_dir.x = m_raycaster->m_dir.x * cos(-rotation_speed) - m_raycaster->m_dir.y * sin(-rotation_speed);
    m_raycaster->m_dir.y = old_dir_x * sin(-rotation_speed) + m_raycaster->m_dir.y * cos(-rotation_speed);
    double old_plane_x = m_raycaster->m_plane.x;
    m_raycaster->m_plane.x = m_raycaster->m_plane.x * cos(-rotation_speed) - m_raycaster->m_plane.y * sin(-rotation_speed);
    m_raycaster->m_plane.y = old_plane_x * sin(-rotation_speed) + m_raycaster->m_plane.y * cos(-rotation_speed);
  }

  // Objects update
  for (auto &pair : m_raycaster->m_objects) {
    auto& obj = *pair.second.get();

    pair.second->onFrameUpdate();

    if (m_raycaster->m_map.getTile(pair.second->getPos().x, pair.second->getPos().y).isSolid()) {
      pair.second->onWallCollision();
    }
  }

  m_raycaster->m_objects.remove_if([](auto& p) { return p.second->isRemovable(); });

  // Sort objects by distance from player
  m_raycaster->m_objects.sort([](const auto& a, const auto& b) { return a.first > b.first; });
}