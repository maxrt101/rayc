#ifndef _RAYC_LAYERS_CONSOLE_H
#define _RAYC_LAYERS_CONSOLE_H

#include <string>
#include <vector>

#include "core/layer.h"
#include "core/input.h"
#include "engine/raycaster.h"
#include "res/font.h"

namespace rayc {

class ConsoleLayer : public Layer, public TextInputListener {
 private:
  struct ConsoleLine {
    std::string text;
    SDL_Color color;
  };

 public:
  ConsoleLayer(Application* app, int w, int h, Raycaster* raycaster);
  ~ConsoleLayer();

  bool onDraw() override;
  void onFrameUpdate(float frame_time) override;
  void onShow() override;
  void onHide() override;

  void onTextInputReady(const std::string& input) override;
  void onTextInputEvent(const std::string& input) override;

  static const std::string Name;

 private:
  void cmd_load(std::vector<std::string>& tokens);
  void cmd_get(std::vector<std::string>& tokens);
  void cmd_set(std::vector<std::string>& tokens);

 private:
  Raycaster* m_raycaster;
  std::vector<ConsoleLine> m_lines;
  int m_max_lines = 20;
  std::string m_prompt = "]";
  std::string m_cmd;
  SDL_Color m_bg_color = {0, 0, 0, 128};
};

};

#endif