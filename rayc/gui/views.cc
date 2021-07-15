#include "gui/views.h"
#include "core/app.h"

rayc::gui::TextureView::TextureView(Texture* texture, Vec2i pos)
    : m_texture(texture) {
  init(pos, texture->getWidth(), texture->getHeight());
}

rayc::gui::TextureView::TextureView(Texture* texture, Vec2i pos, int width, int height)
    : m_texture(texture) {
  init(pos, width, height);
}

rayc::gui::TextureView::~TextureView() {}

void rayc::gui::TextureView::draw(Vec2i abs_parent_offset) {
  m_dest.w = getWidth();
  m_dest.h = getHeight();
  m_dest.x = getX() + abs_parent_offset.x;
  m_dest.y = getY() + abs_parent_offset.y;

  SDL_RenderCopy(Application::getRenderer(), m_texture->getSdlTexture(), NULL, &m_dest);
}

rayc::gui::TextView::TextView(Font* font, int max_height) {
  m_font = font;
  m_max_height = max_height;
}

rayc::gui::TextView::~TextView() {}

void rayc::gui::TextView::pushLine(const std::string& line, SDL_Color color) {
  m_lines.push_back({line, color});
}

void rayc::gui::TextView::popFront() {
  m_lines.erase(m_lines.begin());
}

void rayc::gui::TextView::popBack() {
  m_lines.pop_back();
}

std::string& rayc::gui::TextView::get(int i) {
  return m_lines[i].text;
}

int rayc::gui::TextView::getSize() const {
  return m_lines.size();
}

void rayc::gui::TextView::draw(Vec2i abs_parent_offset) {
  int text_y_offset = 0;
     
  if (m_lines.size() < m_max_height) {
    for (auto &entry : m_lines) {
      m_font->draw(
        entry.text.c_str(),
        {0, text_y_offset},
        entry.color
      );
      text_y_offset += m_font->getSize();
    }
  } else {
    for (int i = m_lines.size()-m_max_height; i < m_lines.size(); i++) {
      m_font->draw(
        m_lines[i].text.c_str(),
        {0, text_y_offset},
        m_lines[i].color
      );
      text_y_offset += m_font->getSize();
    }
  }
}
