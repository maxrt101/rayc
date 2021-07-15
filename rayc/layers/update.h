#ifndef _RAYC_LAYERS_INPUT_H
#define _RAYC_LAYERS_INPUT_H

#include "core/layer.h"
#include "math/vec2.h"
#include "engine/raycaster.h"

namespace rayc {

/* Game State Update Layer */
class UpdateLayer : public Layer {
 public:
  UpdateLayer(Application* app, int w, int h, Raycaster* raycaster);
  ~UpdateLayer();

  bool onDraw() override;
  void onFrameUpdate(float frame_time) override;

  static const std::string Name;

 private:
  Raycaster* m_raycaster;
  float m_last_frame_time;
};

} /* namespace rayc */

#endif