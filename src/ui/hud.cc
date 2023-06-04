#include <rayc/ui/hud.h>
#include <rayc/app.h>
#include <rayc/renderer/color.h>
#include <rayc/data/resources.h>
#include <rayc/game/raycasting.h>
#include <rayc/game/state.h>

rayc::ui::HUD::HUD() : View("hud") {}

rayc::ui::HUD::~HUD() {}

void rayc::ui::HUD::onFrameUpdate(float frameTime) {
  auto rendererView = (Renderer*) app::getView("renderer");

  if (!rendererView || !rendererView->isActive()) {
    return;
  }

  // if (((Renderer*) app::getView("renderer"))->isActive()) { // TODO: null check
    char healthBuffer[5] = {0};
    snprintf(healthBuffer, 5, "%4d", getPlayer().health);
    getFont("main2")->draw(
      std::string(healthBuffer),
      {
        app::getWidth() - getFont("main2")->getWidth() * 5,
        app::getHeight() - getFont("main2")->getHeight() * 2
      },
      RGB_RED
    );

    if (getPlayer().getWeapon()) {
      char weaponBuffer[16] = {0};
      snprintf(weaponBuffer, 16, "%s", getPlayer().getWeapon()->displayName.c_str());
      getFont("main2")->draw(
        std::string(weaponBuffer),
        {
          getFont("main2")->getWidth(),
          app::getHeight() - getFont("main2")->getHeight() * 2
        },
        RGB_WHITE
      );
    }
  // }
}

void rayc::ui::HUD::onEvent(const std::string& eventName, std::map<std::string, std::string> args) {}
