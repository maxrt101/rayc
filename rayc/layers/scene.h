#ifndef _RAYC_LAYERS_SCENE_H
#define _RAYC_LAYERS_SCENE_H

#include "core/layer.h"
#include "engine/raycaster.h"
#include "layers/update.h"

namespace rayc {

class SceneLayer : public Layer {
 public:
  SceneLayer(Application* app, int w, int h, Raycaster* raycaster);
  ~SceneLayer();

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