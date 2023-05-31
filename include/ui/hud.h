#ifndef _RAYC_UI_HUD_H_
#define _RAYC_UI_HUD_H_ 1

#include <rayc/view.h>

namespace rayc {
namespace ui {

class HUD : public View {
 public:
  HUD();
  ~HUD();

  void onFrameUpdate(float frameTime) override;
  void onEvent(const std::string& eventName, std::map<std::string, std::string> args) override;
};

} /* namespace rayc */
} /* namespace rayc */

#endif /* _RAYC_UI_HUD_H_ */