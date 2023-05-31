#include <rayc/view.h>
#include <rayc/app.h>
#include <rayc/log.h>

rayc::View::View(const std::string& name) : mName(name) {}

rayc::View::~View() {}

std::string rayc::View::getName() const {
  return mName;
}

bool rayc::View::isActive() const {
  return mIsActive;
}

void rayc::View::setActive(bool isActive) {
  mIsActive = isActive;
}

void rayc::View::onFrameUpdate(float frameTime) {}

void rayc::View::onEvent(const std::string& eventName, std::map<std::string, std::string> args) {
  warning("(%s) Unhandled event '%s'", mName.c_str(), eventName.c_str());
}
