#ifndef _RAYC_LAYERS_BASE_H
#define _RAYC_LAYERS_BASE_H

#include "core/layer.h"
#include "engine/raycaster.h"

namespace rayc {

class BaseLayer : public rayc::Layer {
 public:
  BaseLayer(rayc::Application* app, int w, int h, rayc::Raycaster* raycaster);

  ~BaseLayer();

  void onPush() override;
  bool onDraw() override;
  void onFrameUpdate(float frame_time) override;

  static const std::string Name;

 private:
  rayc::Raycaster* m_raycaster;
};

} /* namespace rayc */

#endif