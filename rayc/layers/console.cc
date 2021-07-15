#include "layers/console.h"
#include "core/app.h"
#include "layers/scene.h"
#include "utils/debug.h"
#include "utils/log.h"

#include <sstream>

const std::string rayc::ConsoleLayer::Name = "console";

rayc::ConsoleLayer::ConsoleLayer(Application* app, int w, int h, Raycaster* raycaster)
  : rayc::Layer(app, Name, w, h, 0, 0),
    m_raycaster(raycaster) {
}

rayc::ConsoleLayer::~ConsoleLayer() {}

bool rayc::ConsoleLayer::onDraw() {
  clearBuffer();

  SDL_Rect console_bg;
  console_bg.x = 0;
  console_bg.y = 0;
  console_bg.w = getWidth();
  console_bg.h = m_raycaster->m_font_small.getSize() * (m_max_lines+1);

  SDL_SetRenderDrawColor(getRenderer(), m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
  SDL_RenderFillRect(getRenderer(), &console_bg);

  int text_y_offset = 0;
     
  if (m_lines.size() < m_max_lines) {
    for (auto &entry : m_lines) {
      m_raycaster->m_font_small.draw(
        entry.text.c_str(),
        {0, text_y_offset},
        entry.color
      );
      text_y_offset += m_raycaster->m_font_small.getSize();
    }
  } else {
    for (int i = m_lines.size()-m_max_lines; i < m_lines.size(); i++) {
      m_raycaster->m_font_small.draw(
        m_lines[i].text.c_str(),
        {0, text_y_offset},
        m_lines[i].color
      );
      text_y_offset += m_raycaster->m_font_small.getSize();
    }
  }
  std::string str = m_prompt + m_cmd;
  m_raycaster->m_font_small.draw(str.c_str(), {0, text_y_offset});
  return true;
}

void rayc::ConsoleLayer::onFrameUpdate(float frame_time) {
  if (getApp()->getKeyState(SDL_SCANCODE_GRAVE).pressed) {
    hide();
  }
}

void rayc::ConsoleLayer::onShow() {
  getApp()->enableTextInput();
  m_cmd.clear();
}

void rayc::ConsoleLayer::onHide() {
  getApp()->disableTextInput();
}

void rayc::ConsoleLayer::onTextInputReady(const std::string& input) {
  debug("CMD: %s", input.c_str());

  m_lines.push_back({input, kYellow});

  std::stringstream iss(input);
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};;

  if (tokens.size() > 0) {
    if (tokens[0] == "quit") {
      getApp()->stop();
    } else if (tokens[0] == "load") {
      cmd_load(tokens);
    } else if (tokens[0] == "get") {
      cmd_get(tokens);
    } else if (tokens[0] == "set") {
      cmd_set(tokens);
    }
  }
  m_cmd.clear();
}

void rayc::ConsoleLayer::cmd_load(std::vector<std::string>& tokens) {
  if (tokens.size() > 1) {
    if (m_raycaster->loadMap(tokens[1])) {
      Layer* ctx = getApp()->getLayer(rayc::SceneLayer::Name);
      if (ctx) {
        ctx->show();
      } else {
        m_lines.push_back({"Scene layer is not present", kRed});
      }
    } else {
        m_lines.push_back({"Failed to load map", kRed});
    }
  } else {
    m_lines.push_back({"Usage: load MAPFILE", kRed});
  }
}

void rayc::ConsoleLayer::cmd_get(std::vector<std::string>& tokens) {
  if (tokens.size() > 1) {
    if (tokens[1] == "ctx") {
      std::stringstream ss;
      ss << getApp()->getCurrentLayer()->getName() << "@" << getApp()->getCurrentLayer();
      m_lines.push_back({ss.str(), kWhite});
    } else if (tokens[1] == "debug") {
      m_lines.push_back({std::to_string(isDebugEnabled()), kWhite});
    } else if (tokens[1] == "pos") {
      m_lines.push_back({std::to_string(m_raycaster->m_player.x) + " " + std::to_string(m_raycaster->m_player.y), kWhite});
    } else if (tokens[1] == "fov") {
      m_lines.push_back({std::to_string(m_raycaster->m_fov), kWhite});
    } else if (tokens[1] == "fps_cap") {
      m_lines.push_back({std::to_string(getApp()->getFpsCap()), kWhite});
    } else if (tokens[1] == "mspeed") {
      m_lines.push_back({std::to_string(m_raycaster->m_movement_speed), kWhite});
    } else if (tokens[1] == "rspeed") {
      m_lines.push_back({std::to_string(m_raycaster->m_rotation_speed), kWhite});
    } else if (tokens[1] == "depth") {
      m_lines.push_back({std::to_string(m_raycaster->m_depth), kWhite});
    } else {
      m_lines.push_back({"Unknown KEY", kRed});
    }
  } else {
    m_lines.push_back({"Usage: get KEY", kRed});
  }
}

void rayc::ConsoleLayer::cmd_set(std::vector<std::string>& tokens) {
  if (tokens.size() > 2) {
    try {
      if (tokens[1] == "debug") {
        setDebug(std::stoi(tokens[2]));
      } else if (tokens[1] == "pos") {
        if (tokens.size() == 4) {
          if (tokens.size() == 4) {
            m_raycaster->m_player.x = std::stof(tokens[2]);
            m_raycaster->m_player.y = std::stof(tokens[3]);
          } else {
            m_lines.push_back({"Expected 2 floats", kRed});
          }
        } else {
          m_lines.push_back({"Usage: set pos X Y", kWhite});
        }
      } else if (tokens[1] == "fov") {
        m_raycaster->m_fov = std::stoi(tokens[2]) * (PI/180);
      } else if (tokens[1] == "fps_cap") {
        getApp()->setFpsCap(std::stoi(tokens[2]));
      } else if (tokens[1] == "mspeed") {
        m_raycaster->m_movement_speed = std::stoi(tokens[2]);
      } else if (tokens[1] == "rspeed") {
        m_raycaster->m_rotation_speed = std::stoi(tokens[2]);
      } else if (tokens[1] == "depth") {
        m_raycaster->m_depth = std::stoi(tokens[2]);
      } else {
        m_lines.push_back({"Unknown KEY", kRed});
      }
    } catch (const std::exception& e) {
      m_lines.push_back({"Invalid argument", kRed});
    }
  } else {
    m_lines.push_back({"Usage: set KEY VALUE[s]", kRed});
  }
}

void rayc::ConsoleLayer::onTextInputEvent(const std::string& input) {
  for (char ch : input) {
    if (ch == '\b') {
      m_cmd.pop_back();
    } else {
      m_cmd.push_back(ch);
    }
  }
}