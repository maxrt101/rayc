#ifndef _RAYC_LAYERS_SCENEDDA_H
#define _RAYC_LAYERS_SCENEDDA_H

#include "core/layer.h"
#include "engine/raycaster.h"
#include "layers/update.h"

namespace rayc {

class SceneDDALayer : public Layer {
 public:
  SceneDDALayer(Application* app, int w, int h, Raycaster* raycaster);
  ~SceneDDALayer();

  bool onDraw() override;
  void onFrameUpdate(float frame_time) override;
  void onShow() override;
  void onHide() override;

  static const std::string Name;

 private:
  Raycaster* m_raycaster;
  UpdateLayer m_update_layer;
};

} /* namespace rayc */

#endif