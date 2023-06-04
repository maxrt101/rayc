#include <rayc/ui/alert.h>
#include <rayc/renderer.h>
#include <rayc/game/raycasting.h>
#include <rayc/data/resources.h>
#include <rayc/utils/string.h>
#include <rayc/app.h>
#include <rayc/log.h>

rayc::ui::Alert::Alert() : View("alert") {}

rayc::ui::Alert::~Alert() {}

void rayc::ui::Alert::addMessage(float duration, const std::string& msg) {
  mMessages.push_back({msg, duration});
}

void rayc::ui::Alert::clearMessages() {
  mMessages.clear();
}

void rayc::ui::Alert::onFrameUpdate(float frameTime) {
  auto rendererView = (Renderer*) app::getView("renderer");

  if (!rendererView || !rendererView->isActive()) {
    return;
  }

  if (!mMessages.empty()) {
    if (mMessages[0].duration > 0) {
      // char healthBuffer[5] = {0};
      // snprintf(healthBuffer, 5, "%4d", getPlayer().health);
      getFont("main3")->draw(
        mMessages[0].message,
        {
          int(app::getWidth() / 2 - getFont("main3")->getWidth() * mMessages[0].message.size() / 2),
          int(app::getHeight() / 2 - getFont("main3")->getHeight() / 2)
        },
        RGB_WHITE
      );
      mMessages[0].duration -= frameTime;
    } else {
      mMessages.erase(mMessages.begin());
    }
  }
}

void rayc::ui::Alert::onEvent(const std::string& eventName, std::map<std::string, std::string> args) {
  for (auto& [k, v] : args) {
    info("k='%s' v='%s'", k.c_str(), v.c_str());
  }

  if (eventName == "add") {
    std::string msg;
    float duration = 100;

    if (args.find("msg") != args.end()) {
      msg = args["msg"];
    } else {
      error("(alert) Missing required event argument 'msg'");
      return;
    }

    if (args.find("duration") != args.end()) {
      rayc::stof(args["duration"], duration);
    } else {
      error("(alert) Missing required event argument 'duration'");
      return;
    }

    addMessage(duration, msg);
  } else if (eventName == "clear") {
    clearMessages();
  } else {
    error("(renderer) Unsupported event '%s'", eventName.c_str());
  }
}
