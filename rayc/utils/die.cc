#include "utils/die.h"
#include <vector>

static std::vector<rayc::OnDieCallback> g_onDieCallbacks;

[[noreturn]] void rayc::die(int exitCode) {
  for (auto cb : g_onDieCallbacks) {
    cb();
  }
  exit(exitCode);
}

void rayc::registerOnDie(OnDieCallback cb) {
  g_onDieCallbacks.push_back(cb);
}
