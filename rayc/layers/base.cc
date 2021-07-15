#include "layers/base.h"
#include "layers/console.h"
#include "core/app.h"
#include "utils/version.h"

const std::string rayc::BaseLayer::Name = "base";

rayc::BaseLayer::BaseLayer(rayc::Application* app, int w, int h, rayc::Raycaster* raycaster)
  : rayc::Layer(app, Name, w, h, 0, 0), m_raycaster(raycaster) {
  show();
}

rayc::BaseLayer::~BaseLayer() {}

void rayc::BaseLayer::onPush() {}

bool rayc::BaseLayer::onDraw() {
  clearBuffer();
  m_raycaster->m_font_big.draw("RAYC v" RAYC_VERSION_STRING " DEV", {10, 10});
  return true;
}

void rayc::BaseLayer::onFrameUpdate(float frame_time) {
  if (getApp()->getKeyState(SDL_SCANCODE_GRAVE).pressed) {
    getApp()->getLayer(rayc::ConsoleLayer::Name)->show();
  }
}